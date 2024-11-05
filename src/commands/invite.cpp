#include "../../include/Server.hpp"

/* https://modern.ircdocs.horse/#invite-message
 * Parameters: <nickname> <channel> */
void Server::_invite(PollfdIterator* it, const std::vector<std::string>& args)
{
    Client& client = m_clients.find((*it)->fd)->second;
    if (args.size() < 3)
        return client.send_461("INVITE");

    const std::string& target_name = args[1];
    const std::string& channel_name = args[2];

    ClientIterator target_client_it = _find_client_by_nickname(target_name);
    if (target_client_it == m_clients.end())
        return client.send_401(target_name);
    Client& target_client = target_client_it->second;

    ChannelIterator target_channel_it = _find_channel(channel_name);
    if (target_channel_it == m_channels.end())
        return client.send_403(channel_name);
    Channel& target_channel = target_channel_it->second;

    if (!target_channel.is_subscribed(client.fd))
        return client.send_442(target_channel);

    if (target_channel.is_subscribed(target_client.fd))
        return client.send_443(target_channel);

    if (!target_channel.is_operator(client.fd))
        return client.send_482(target_channel);

    target_channel.invited_users_fd.push_back(target_client.fd);
    std::string msg =
        ":" + client.nickname + " INVITE " + target_client.nickname + " :" + target_channel.name;
    client.send_341(target_channel, client.nickname);
    client.send_msg(msg);
    target_client.send_msg(msg);
}
