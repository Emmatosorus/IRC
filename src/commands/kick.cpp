#include "../../include/Server.hpp"
#include "../../include/utils.hpp"
#include <string>

/* https://modern.ircdocs.horse/#kick-message
 * Parameters: <channel> <user> *( "," <user> ) [<comment>] */
void Server::_kick(PollfdIterator* it, const std::vector<std::string>& args)
{
    Client& client = m_clients[(*it)->fd];
    if (args.size() < 3)
        return client.send_461("KICK");

    const std::string& channel_name = args[1];

    ChannelIterator target_channel = _find_channel(channel_name);
    if (target_channel == m_channels.end())
        return client.send_403(args[1]);
    Channel& channel = target_channel->second;

    if (!channel.is_subscribed(client.fd))
        return client.send_442(target_channel->second);

    if (!channel.is_operator(client.fd))
        return client.send_482(target_channel->second);

    std::vector<std::string> targets = parse_comma_arg(args[2]);
    std::string message = "You have been kicked";
    if (args.size() > 3)
        message = args[3];
    for (size_t i = 0; i < targets.size(); i++)
    {
        ClientIterator target_it = _find_client_by_nickname(targets[i]);
        if (target_it == m_clients.end())
        {
            client.send_401(targets[i]);
            continue;
        }

        Client& target = target_it->second;
        if (!channel.is_subscribed(target.fd))
        {
            client.send_442(channel);
            continue;
        }
        std::string msg = ":" + client.nickname + " KICK " + channel.name + " " + target.nickname;
        if (args.size() > 3)
            msg += " :" + args[3];
        channel.send_msg(msg);
        _remove_client_from_channel(channel, target);
    }
}
