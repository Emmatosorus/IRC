#include "../../include/Server.hpp"

/* https://modern.ircdocs.horse/#ping-message
 * Parameters: <token> */
void Server::_ping(PollfdIterator* it, const std::vector<std::string>& args)
{
    Client& client = m_clients[(*it)->fd];
    if (args.size() != 2)
        return client.send_461("PING");

    client.send_msg(":42Chan PONG 42Chan :" + args[1]);
}
