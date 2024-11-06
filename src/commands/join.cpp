#include "../../include/Server.hpp"
#include "../../include/utils.hpp"
#include <algorithm>

static std::string _is_channel_name_valid(const std::string& channel_name);

/* https://modern.ircdocs.horse/#join-message
 * Parameters: <channel>{,<channel>} [<key>{,<key>}] */
void Server::_join(PollfdIterator* it, const std::vector<std::string>& args)
{
    Client& client = m_clients[(*it)->fd];
    if (args.size() < 2)
        return client.send_461("JOIN");

    if (args[1] == "0")
        return _part_all_channels(client);

    std::vector<std::string> channels_to_join = parse_comma_arg(args[1]);
    std::vector<std::string> password_to_channels = parse_comma_arg(args[2]);
    password_to_channels.reserve(channels_to_join.size());
    while (password_to_channels.size() < channels_to_join.size())
        password_to_channels.push_back("");

    for (size_t i = 0; i < channels_to_join.size(); i++)
    {
        const std::string& channel_name = channels_to_join[i];
        const std::string channel_name_error = _is_channel_name_valid(channels_to_join[i]);
        if (channel_name_error != "")
        {
            client.send_448(channel_name, channel_name_error);
            continue;
        }

        ChannelIterator target_channel = _find_channel(channel_name);
        if (target_channel == m_channels.end())
        {
            Channel new_channel(client.fd, channel_name);
            m_channels.insert(std::make_pair(channel_name, new_channel));
            _add_client_to_channel(new_channel, client, false);
            continue;
        }

        Channel& channel = target_channel->second;
        if (channel.is_subscribed(client.fd))
            continue;
        if (client.channels.size() > MAX_CHANNELS)
        {
            client.send_448(channel_name, "Client joined too many channels");
            continue;
        }
        else if (channel.is_invite_only_mode && !channel.is_invited(client.fd))
        {
            client.send_473(channel);
            continue;
        }
        else if (channel.is_password_mode && channel.password != password_to_channels[i])
        {
            client.send_475(channel);
            continue;
        }
        else if (channel.is_user_limit_mode &&
                 channel.subscribed_users_fd.size() + 1 > channel.user_limit)
        {
            client.send_471(channel);
            continue;
        }

        _add_client_to_channel(channel, client, true);
    }
}

void Server::_add_client_to_channel(Channel& channel, Client& client, bool should_add)
{
    if (should_add)
        channel.subscribed_users_fd.push_back(client.fd);
    client.channels.push_back(channel.name);
    channel.send_msg(":" + client.nickname + " JOIN :" + channel.name);
    if (channel.topic != "")
    {
        client.send_332(channel);
        client.send_333(channel);
    }
    client.send_353(channel, channel.get_list_of_clients(m_clients));
    client.send_366(channel.name);
    client.send_329(channel);
    if (channel.is_invite_only_mode)
    {
        std::vector<int>::iterator it =
            std::find(channel.invited_users_fd.begin(), channel.invited_users_fd.end(), client.fd);
        channel.invited_users_fd.erase(it);
    }
}

static std::string _is_channel_name_valid(const std::string& channel_name)
{
    if (channel_name[0] != '#')
        return "Channel name must start with a hash mark";
    if (channel_name.length() < 2)
        return "Channel name is too short";
    if (channel_name.length() > MAX_CHANNEL_NAME)
        return "Channel name is too long";
    if (channel_name.find_first_of(": ,\a\t\r\n") != std::string::npos)
        return "Invalid channel character: : ,\\a\\t\\r\\n";
    return "";
}

void Server::_part_all_channels(Client& client)
{
    while (!client.channels.empty())
    {
        _part_channel(client, m_channels[client.channels[0]], "");
    }
}
