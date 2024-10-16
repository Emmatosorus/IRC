#include "../../include/Server.hpp"
#include "../../include/utils.hpp"
#include <string>

/* https://modern.ircdocs.horse/#privmsg-message
 * Parameters: <target>{,<target>} <text to be sent> */
void Server::_privmsg(PollfdIterator it, const std::vector<std::string>& args)
{
	Client& client = m_clients[it->fd];

	if (args.size() > 3)
		return client.send_407("Too many targets");
	if (args.size() == 1)
		return client.send_411();
	if (args.size() == 2)
		return client.send_412();

	std::vector<std::string> targets = parse_comma_arg(args[1]);
	for (size_t i = 0; i < targets.size(); i++)
	{
		std::map<std::string, Channel>::iterator target_channel_it = m_channels.find(targets[i]);
		if (target_channel_it != m_channels.end())
		{
			Channel& target_channel = target_channel_it->second;
			if (!target_channel.is_subscribed(client.fd))
			{
				client.send_441(target_channel.name);
				continue;
			}
			target_channel.send_msg_except(client.fd, ":" + client.nickname + " PRIVMSG " + target_channel.name + " :" + args[2]);
			continue;
		}
		ClientIterator target_user_it = _find_client_by_nickname(targets[i]);
		if (target_user_it != m_clients.end())
		{
			Client& target_user = target_user_it->second;
			target_user.send_msg(":" + client.nickname + " PRIVMSG " + target_user.nickname + " :" + args[2]);
			continue;
		}
		client.send_401(targets[i]);
	}
}
