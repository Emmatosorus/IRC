#include "../../include/Server.hpp"
#include "../../include/utils.hpp"

/* https://modern.ircdocs.horse/#privmsg-message
 * Parameters: <target>{,<target>} <text to be sent> */
void Server::_privmsg(PollfdIterator* it, const std::vector<std::string>& args)
{
    Client& client = m_clients[(*it)->fd];
    if (args.size() == 1)
        return client.send_411();
    if (args.size() == 2)
        return client.send_412();

    std::vector<std::string> targets = parse_comma_arg(args[1]);
    make_unique(targets);
    std::string msg = args[2];
    if (args.size() > 3)
        for (size_t i = 3; i < args.size(); i++)
            msg += " " + args[i];
    for (size_t i = 0; i < targets.size(); i++)
    {
        bool is_to_operators = false;
        if (targets[i].size() > 2 && targets[i][0] == '@')
        {
            is_to_operators = true;
            targets[i].erase(0, 2);
        }

        ChannelIterator target_channel_it = _find_channel(targets[i]);
        if (target_channel_it != m_channels.end())
        {
            Channel& target_channel = target_channel_it->second;
            if (!target_channel.is_subscribed(client.fd) &&
                target_channel.is_no_external_messages_mode)
            {
                client.send_441(target_channel.name);
                continue;
            }
            if (is_to_operators)
            {
                target_channel.send_msg_to_operators(client.fd,
                                                     ":" + client.nickname + " PRIVMSG " +
                                                         target_channel.name + " :" + msg);
                continue;
            }
            target_channel.send_msg_except(client.fd, ":" + client.nickname + " PRIVMSG " +
                                                          target_channel.name + " :" + msg);
            continue;
        }
        ClientIterator target_user_it = _find_client_by_nickname(targets[i]);
        if (target_user_it != m_clients.end())
        {
            Client& target_user = target_user_it->second;
            if (target_user.away_msg != "")
            {
                client.send_301(target_user.nickname, target_user.away_msg);
                continue;
            }
            target_user.send_msg(":" + client.nickname + " PRIVMSG " + target_user.nickname + " :" +
                                 msg);
            continue;
        }
        client.send_401(targets[i]);
    }
}
