#include "../../include/Server.hpp"
#include "../../include/utils.hpp"
#include <algorithm>
#include <climits>
#include <cerrno>

/* https://modern.ircdocs.horse/#mode-message
 * Parameters: <target> [<modestring> [<mode arguments>...]] */
void Server::_mode(PollfdIterator* it, const std::vector<std::string>& args)
{
    Client& client = m_clients[(*it)->fd];
    if (args.size() < 2)
        return client.send_461("MODE");

    if (args[1][0] == '#')
        _resolve_channel_mode(args, client);
    else
        _resolve_user_mode(args, client);
}

void Server::_resolve_channel_mode(const std::vector<std::string>& args, Client& client)
{
    const std::string& channel_name = args[1];

    ChannelIterator target_channel = _find_channel(channel_name);
    if (target_channel == m_channels.end())
        return client.send_403(args[1]);
    Channel& channel = target_channel->second;

    if (!channel.is_subscribed(client.fd))
        return client.send_442(target_channel->second);

    if (args.size() == 2)
    {
        client.send_324(channel);
        client.send_329(channel);
        return;
    }

    if (!channel.is_operator(client.fd))
        return client.send_482(target_channel->second);

    std::string added_modes;
    std::string added_modes_args;
    std::string removed_modes;
    size_t j = 3;
    bool is_add_mode = true;
    for (size_t i = 0; i < args[2].size(); i++)
    {
        switch (args[2][i])
        {
        case '-':
            is_add_mode = false;
            break;
        case '+':
            is_add_mode = true;
            break;
        case 'i':
            channel.is_invite_only_mode = is_add_mode;
            added_modes += is_add_mode ? "i" : "";
            removed_modes += !is_add_mode ? "i" : "";
            break;
        case 't':
            channel.is_const_topic_mode = is_add_mode;
            added_modes += is_add_mode ? "t" : "";
            removed_modes += !is_add_mode ? "t" : "";
            break;
        case 'n':
            channel.is_no_external_messages_mode = is_add_mode;
            added_modes += is_add_mode ? "n" : "";
            removed_modes += !is_add_mode ? "n" : "";
            break;
        case 'k':
            if (is_add_mode && j > args.size() - 1)
            {
                client.send_696(channel.name, 'k', "", "No argument given");
                continue;
            }
            _mode_k(added_modes, added_modes_args, removed_modes, is_add_mode, args[j], client,
                    channel);
            if (is_add_mode)
                j++;
            break;
        case 'o':
            if (j > args.size() - 1)
            {
                client.send_696(channel.name, 'o', "", "No argument given");
                continue;
            }
            _mode_o(is_add_mode, args[j], channel, client);
            if (is_add_mode)
                j++;
            break;
        case 'l':
            if (is_add_mode && j > args.size() - 1)
            {
                client.send_696(channel.name, 'l', "", "No argument given");
                continue;
            }
            _mode_l(added_modes, added_modes_args, removed_modes, is_add_mode, args[j], channel,
                    client);
            if (is_add_mode)
                j++;
            break;
        default:
            client.send_472(args[2][i]);
            continue;
        }
    }
    std::string mode_string_to_send;
    if (removed_modes != "")
        mode_string_to_send += "-" + removed_modes;
    if (added_modes != "")
        mode_string_to_send += "+" + added_modes + added_modes_args;
    if (mode_string_to_send != "")
        channel.send_msg(":" + client.nickname + " MODE " + channel.name + " :" +
                         mode_string_to_send);
}

void Server::_resolve_user_mode(const std::vector<std::string>& args, Client& client)
{
    if (to_irc_lower_case(args[1]) != to_irc_lower_case(client.nickname))
        return client.send_502();
    client.send_221();
}

/* Set/removes channel password */
void Server::_mode_k(std::string& added_modes, std::string& added_modes_args,
                     std::string& removed_modes, bool is_add_mode,
                     const std::string& channel_password, Client& client, Channel& channel)
{
    if (!is_add_mode)
    {
        channel.is_password_mode = false;
        channel.password.clear();
        removed_modes += 'k';
        return;
    }
    if (channel_password.find_first_of(" %@#,") != std::string::npos)
    {
        client.send_525(channel);
        return;
    }
    added_modes += 'k';
    added_modes_args += " " + channel_password;
    channel.is_password_mode = true;
    channel.password = channel_password;
}

/* set/removes user limit on channel */
void Server::_mode_l(std::string& added_modes, std::string& added_modes_args,
                     std::string& removed_modes, bool is_add_mode, const std::string& user_limit,
                     Channel& channel, Client& client)
{
    if (is_add_mode)
    {
		long res = strtol(user_limit.c_str(), NULL, 10);
		if (res < 1 || res > INT_MAX)
			return client.send_696(channel.name, 'l', user_limit, "User limit must be greater than 0 and lower than 2147483647");

        channel.is_user_limit_mode = true;
        size_t limit = std::atoi(user_limit.c_str());
		added_modes += 'l';
		added_modes_args += " " + user_limit;
		channel.user_limit = limit;
		return;
    }
    removed_modes += 'l';
    channel.is_user_limit_mode = false;
    channel.user_limit = 0;
}

/* Give/remove operator priviliges */
void Server::_mode_o(bool is_add_mode, const std::string& nickname, Channel& channel,
                     Client& client)
{
    ClientIterator target_it = _find_client_by_nickname(nickname);
    if (target_it == m_clients.end())
        return client.send_696(channel.name, 'o', nickname, "Unknown user");

    Client& target = target_it->second;
    if (!channel.is_subscribed(target.fd))
        return client.send_696(channel.name, 'o', nickname, "User is not subscribed to channel");

    if (is_add_mode)
    {
        channel.channel_operators_fd.push_back(target.fd);
        channel.send_msg(":" + client.nickname + " MODE " + channel.name + " +o " +
                         target.nickname);
        return;
    }
    std::vector<int>::iterator it = std::find(channel.channel_operators_fd.begin(),
                                              channel.channel_operators_fd.end(), target.fd);
    channel.channel_operators_fd.erase(it);
    channel.send_msg(":" + client.nickname + " MODE " + channel.name + " -o " + target.nickname);
}
