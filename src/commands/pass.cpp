#include "../../include/Server.hpp"
#include <iostream>

/* https://modern.ircdocs.horse/#pass-message
 * Parameters: <password> */
void Server::_pass(PollfdIterator it, const std::vector<std::string>& args)
{
	if (args.size() != 2)
	{
		Server::_send_to_client(it, "461", "Invalid number of parameters :\nPASS <password>");
		return ;
	}
	std::map<int, Client>::iterator connected_client = m_clients.find(it->fd);
	if (connected_client->second.is_registered)
	{
		_send_to_client(it, "462", "You are already registered");
		return;
	}
	connected_client->second.password = args[1];
	connected_client->second.entered_password = true;
}
