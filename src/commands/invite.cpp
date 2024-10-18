#include "../../include/Server.hpp"

/* https://modern.ircdocs.horse/#invite-message
 * Parameters: <nickname> <channel> */
void Server::_invite(PollfdIterator* it, const std::vector<std::string>& args)
{
    Client& client = m_clients.find((*it)->fd)->second;
    if (args.size() < 2)
		return client.send_461("INVITE");

    const std::string& target_name = args[1];
    const std::string& channel_name = args[2];
    bool is_operator = false;


    std::map<std::string, Channel>::iterator target_channel = m_channels.find(channel_name);
    if (target_channel != m_channels.end())
    	return;
    std::map<int, Client>::iterator client_it = m_clients.find((*it)->fd);
    std::map<int, Client>::iterator target_client = _find_client_by_nickname(target_name);

    
    if (_check_invite_args(args, client) != 0)
        return;
    if (_check_presence(target_channel, client_it, is_operator) != 0)
    {
        client.send_442(target_channel->second);
        return;
    }
    if (target_channel->second.is_invite_only_mode && !is_operator)
    {
        client.send_482(target_channel->second);
        return;
    }
    if (_check_presence(target_channel, target_client, is_operator) != 0)
    {
        client.send_443(target_channel->second);
        return;
    }
    target_channel->second.invited_users_fd.push_back(client.fd);
    client.send_341(target_channel->second, client.nickname);
    client.send_msg(client.nickname + " INVITE " + client.nickname + " :" + target_channel->second.name);
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

int Server::_check_invite_args(const std::vector<std::string>& args, Client& client)
{
    if (args.size() < 3)
    {
        client.send_461(args[0]);
        return (1);
    }
    return (0);
}
