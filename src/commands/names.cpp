#include "../../include/Server.hpp"
#include "../../include/utils.hpp"

/* https://modern.ircdocs.horse/#names-message
 * Parameters: <channel>{,<channel>} */
void Server::_names(PollfdIterator* it, const std::vector<std::string>& args)
{
    Client& client = m_clients.find((*it)->fd)->second;
    if (args.size() < 2)
        return client.send_461("NAMES");

    std::vector<std::string> targets = parse_comma_arg(args[1]);
    for (size_t i = 0; i < targets.size(); i++)
    {
        const std::string& channel_name = targets[i];
        std::map<std::string, Channel>::iterator target_channel = m_channels.find(channel_name);

        if (target_channel == m_channels.end())
        {
            client.send_366(channel_name);
            continue;
        }

        Channel& channel = target_channel->second;
        std::string list_of_clients;
        for (std::vector<int>::iterator it = channel.subscribed_users_fd.begin();
             it != channel.subscribed_users_fd.end(); it++)
        {
            const Client& client = m_clients.find(*it)->second;
            if (channel.is_operator(client.fd))
                list_of_clients += "@";
            list_of_clients += client.nickname;
            if (it + 1 != channel.subscribed_users_fd.end())
                list_of_clients += " ";
        }
        client.send_353(channel, list_of_clients);
        client.send_366(channel_name);
    }
}
