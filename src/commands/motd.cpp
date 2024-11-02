#include "../../include/Server.hpp"

/* https://modern.ircdocs.horse/#ping-message
 * Parameters: <token> */
void Server::_motd(PollfdIterator* it, const std::vector<std::string>& args)
{
	(void)args;
    Client& client = m_clients[(*it)->fd];

    client.send_372();
}
