#include "../../include/Server.hpp"
#include "../../include/utils.hpp"

/* https://modern.ircdocs.horse/#names-message
 * Parameters: <channel>{,<channel>} */
void Server::_names(PollfdIterator* it, const std::vector<std::string>& args)
{
    Client& client = m_clients.find((*it)->fd)->second;
    if (args.size() < 2)
    {
        ChannelIterator channel_it = m_channels.begin();
        while (channel_it != m_channels.end())
        {    
            Channel& channel = channel_it->second;
            client.send_353(channel, channel.get_list_of_clients(m_clients));
            channel_it++;
        }
        ClientIterator client_it = m_clients.begin();
        std::string    list_of_clients;
        while (client_it != m_clients.end())
        {
            Client& client = client_it->second;
            if (client.channels.empty())
                list_of_clients += client.nickname + " ";
            client_it++;
        }
        if (!list_of_clients.empty())
        {
            list_of_clients.erase(--list_of_clients.end());
            client.send_353(list_of_clients);
        }
        client.send_366("*");
        return;
    }

    std::vector<std::string> targets = parse_comma_arg(args[1]);
    make_unique(targets);
    for (size_t i = 0; i < targets.size(); i++)
    {
        const std::string& channel_name = targets[i];
        ChannelIterator target_channel = _find_channel(channel_name);

        if (target_channel == m_channels.end())
        {
            client.send_366(channel_name);
            continue;
        }
        Channel& channel = target_channel->second;
        client.send_353(channel, channel.get_list_of_clients(m_clients));
        client.send_366(channel_name);
    }
}
