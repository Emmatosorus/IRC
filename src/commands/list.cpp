#include "../../include/Server.hpp"

/* https://modern.ircdocs.horse/#user-message
 * Parameters: <username> 0 * : <realname> */
void Server::_list(PollfdIterator* it, const std::vector<std::string>& args)
{
	(void)args;
	Client& client = m_clients[(*it)->fd];

	for (ChannelIterator it = m_channels.begin(); it != m_channels.end(); it++)
	{
		client.send_322(it->second);
	}
	client.send_323();
}
