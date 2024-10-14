#include "../../include/Server.hpp"
#include "../../include/utils.hpp"

static std::string _join_message(const Client& client, const Channel& channel);

/* https://modern.ircdocs.horse/#join-message
 * Parameters: <channel>{,<channel>} [<key>{,<key>}] */
void Server::_join(PollfdIterator it, const std::vector<std::string>& args)
{
	Client& client = m_clients.find(it->fd)->second;
	if (args.size() < 2)
		return client.send_461("JOIN");

	std::vector<std::string> channels_to_join = get_all_targets(args[1]);
	std::vector<std::string> password_to_channels = get_all_targets(args[2]);
	password_to_channels.reserve(channels_to_join.size());
	while (password_to_channels.size() < channels_to_join.size())
		password_to_channels.push_back("");

	for (size_t i = 0; i < channels_to_join.size(); i++)
	{
		const std::string& channel_name = channels_to_join[i];
		if (channel_name[0] != '#')
		{
			client.send_448(channel_name, "Channel name must start with a hash mark");
			continue;
		}

		std::map<std::string, Channel>::iterator target_channel = m_channels.find(channel_name);
		if (target_channel == m_channels.end())
		{
			Channel new_channel(client.fd, channel_name);
			m_channels.insert(std::make_pair(channel_name, new_channel));
			_join_channel(it, new_channel, client, false);
			continue;
		}

		Channel& channel = target_channel->second;
		if (channel.is_subscribed(client.fd))
			continue;
		else if (channel.is_invite_only_mode)
		{
			client.send_473(channel);
			continue;
		}
		else if  (channel.is_password_mode && channel.password != password_to_channels[i])
		{
			client.send_475(channel);
			continue;
		}
		else if (channel.is_user_limit_mode && channel.subscribed_users_fd.size() + 1 > channel.user_limit)
		{
			client.send_471(channel);
			continue;
		}

		_join_channel(it, channel, client, true);
	}
}

void Server::_join_channel(PollfdIterator it, Channel& channel, const Client& client, bool should_add)
{
	if (should_add)
		channel.subscribed_users_fd.push_back(client.fd);
	channel.send_msg(_join_message(client, channel));
	if (channel.topic != "")
	{
		std::vector<std::string> topic_args;
		topic_args.push_back("TOPIC");
		topic_args.push_back(channel.name);
		_topic(it, topic_args);
	}
	std::vector<std::string> names_args;
	names_args.push_back("NAMES");
	names_args.push_back(channel.name);
	_names(it, names_args);
}

static std::string _join_message(const Client& client, const Channel& channel)
{
	return ":" + client.nickname + " JOIN :" + channel.name;
}
