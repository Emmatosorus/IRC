#include "../../include/Server.hpp"

/* https://modern.ircdocs.horse/#quit-message
 * Parameters: [<reason>] */
void Server::_quit(PollfdIterator* it, const std::vector<std::string>& args)
{
	Client& client = m_clients[(*it)->fd];

	std::string quit_msg = ":" + client.nickname + " QUIT";
	if (args.size() > 1)
		quit_msg + ":" + args[1];

	for (size_t i = 0; i < client.channels.size(); i++)
	{
		Channel& target_channel = m_channels[client.channels[i]];
		target_channel.send_msg(quit_msg);
	}
	_remove_client(it, client);
}
