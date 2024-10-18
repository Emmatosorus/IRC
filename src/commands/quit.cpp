#include "../../include/Server.hpp"
#include <iostream>

/* https://modern.ircdocs.horse/#quit-message
 * Parameters: [<reason>] */
void Server::_quit(PollfdIterator* it, const std::vector<std::string>& args)
{
	Client& client = m_clients[(*it)->fd];

	std::string quit_message = ": " + client.nickname + " QUIT";
	if (args.size() > 1)
		quit_message + ":" + args[1];

	_remove_client(it);
}
