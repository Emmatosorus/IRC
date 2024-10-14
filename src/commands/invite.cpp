#include "../../include/Server.hpp"
#include <iostream>

/* https://modern.ircdocs.horse/#invite-message
 * Parameters: <nickname> <channel> */
void Server::_invite(PollfdIterator it, const std::vector<std::string>& args)
{
    if (_check_invite_args(it, args) != 0)
        return;

    const std::string& target_name = args[1];
    const std::string& channel_name = args[2];
    bool is_operator = false;


    std::map<std::string, Channel>::iterator target_channel = m_channels.find(channel_name);
    if (target_channel != m_channels.end())
    	return;
    std::map<int, Client>::iterator client = m_clients.find(it->fd);
    std::map<int, Client>::iterator target_client = _find_client_by_nickname(target_name);
    if (client != m_clients.end())
        return;
    if (_check_invite_args(it, args) != 0)
        return;
    if (_check_presence(target_channel, client, is_operator) != 0)
    {
        _send_to_client(client->second.fd, "442", "You are not on the channel");
        return;
    }
    if (target_channel->second.is_invite_only_mode && !is_operator)
    {
        _send_to_client(client->second.fd, "482", "Only operators can invite to this channel");
        return;
    }
    if (_check_presence(target_channel, target_client, is_operator) != 0)
    {
        _send_to_client(client->second.fd, "443", "User is already on channel");
        return;
    }
    target_channel->second.subscribed_users_fd.push_back(target_client->second.fd);
    _send_to_client(client->second.fd, "341", target_name + " has been invited");
    _send_to_client(target_client->second.fd, "341", "You have been invited to " + channel_name);
}

int Server::_check_presence(std::map<std::string, Channel>::iterator target_channel,  std::map<int, Client>::iterator client, bool & is_operator)
{
    for (std::vector<int>::iterator it = target_channel->second.channel_operators_fd.begin(); it != target_channel->second.channel_operators_fd.end(); it++)
    {
        if (*it == client->second.fd)
        {
            is_operator = true;
            return 0;
        }
    }
    for (std::vector<int>::iterator it = target_channel->second.subscribed_users_fd.begin(); it != target_channel->second.subscribed_users_fd.end(); it++)
        if (*it == client->second.fd)
            return 0;
    return (1);
}

int Server::_check_invite_args(PollfdIterator it, const std::vector<std::string>& args)
{
    if (args.size() < 3)
    {
        _send_to_client(it->fd, "461", "Not enough parameters :\nINVITE <nickname> <channel>");
        return (1);
    }
    if (args.size() > 3)
    {
        _send_to_client(it->fd, "461", "Too many parameters :\nINVITE <nickname> <channel>");
        return (1);
    }
    return (0);
}