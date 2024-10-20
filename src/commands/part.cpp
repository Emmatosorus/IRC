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
    for (size_t i = 0; i < channels_to_part.size(); i++)
    {
        const std::string& channel_name = channels_to_part[i];

        std::map<std::string, Channel>::iterator target_channel = m_channels.find(channel_name);
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

		std::string msg = ":" + client.nickname + " PART " + channel.name;
		if (args.size() > 2)
			msg += " :" + args[2];
		channel.send_msg(msg);
        _remove_client_from_channel(channel, client);
    }
}
// :emma!emma@46.231.218.157 PART #dogs :don't like dogs
// :emma!emma@46.231.218.157 PART #dogs
