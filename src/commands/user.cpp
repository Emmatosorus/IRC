#include "../../include/Server.hpp"

/* https://modern.ircdocs.horse/#user-message
 * Parameters: <username> 0 * : <realname> */
void Server::_user(PollfdIterator* it, const std::vector<std::string>& args)
{
    Client& client = m_clients[(*it)->fd];
    if (args.size() < 5)
        return client.send_461("USER");

    if (client.is_registered)
        return client.send_462();

    if (args[1].size() > USERLEN)
        return client.send_468("username is too long");

    size_t pos = args[1].find_first_of("#%:,*?!@.\t\r\n ");
    if (pos != std::string::npos)
        return client.send_468("username contains invalid characters: #%:,*?!@. \\t\\r\\n");

    client.username = args[1];
    client.fullname = args[4];
    if (client.nickname != "")
    {
        if (client.password != m_password)
            return client.send_464();

        _register_user(client);
    }
}

void Server::_register_user(Client& client)
{
    client.is_registered = true;
    client.send_001();
    client.send_002();
    client.send_003(m_creation_time);
    client.send_004();
    client.send_375();
    client.send_372();
    client.send_376();
    if (m_channels.size() == 0)
        return;
    for (ChannelIterator it = m_channels.begin(); it != m_channels.end(); it++)
        client.send_322(it->second);
    client.send_323();
    return;
}
