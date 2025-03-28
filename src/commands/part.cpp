#include "../../include/Server.hpp"
#include "../../include/utils.hpp"

/* https://modern.ircdocs.horse/#part-message
 * Parameters: <channel>{,<channel>} [<reason>] */
void Server::_part(PollfdIterator* it, const std::vector<std::string>& args)
{
    Client& client = m_clients[(*it)->fd];
    if (args.size() < 2)
        return client.send_461("PART");

    std::vector<std::string> channels_to_part = parse_comma_arg(args[1]);
    std::string reason = "";
    if (args.size() > 2)
        reason = args[2];
    for (size_t i = 0; i < channels_to_part.size(); i++)
    {
        const std::string& channel_name = channels_to_part[i];

        ChannelIterator target_channel = _find_channel(channel_name);
        if (target_channel == m_channels.end())
        {
            client.send_403(channels_to_part[i]);
            continue;
        }
        Channel& channel = target_channel->second;

        if (!channel.is_subscribed(client.fd))
        {
            client.send_442(channel);
            continue;
        }

        _part_channel(client, channel, reason);
    }
}

void Server::_part_channel(Client& client, Channel& channel, const std::string& reason)
{
    std::string msg = ":" + client.nickname + " PART " + channel.name;
    if (reason != "")
        msg += " :" + reason;
    channel.send_msg(msg);
    _remove_client_from_channel(channel, client);
}
