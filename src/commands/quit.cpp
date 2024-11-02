#include "../../include/Server.hpp"

/* https://modern.ircdocs.horse/#quit-message
 * Parameters: [<reason>] */
void Server::_quit(PollfdIterator* it, const std::vector<std::string>& args)
{
    Client& client = m_clients[(*it)->fd];

    if (args.size() > 1)
        return _quit_client(it, client, args[1]);

    _quit_client(it, client, "");
}
