#include "../../include/Server.hpp"
#include "../../include/utils.hpp"

static std::string _topic_message(const Client& client, const Channel& channel);

/* https://modern.ircdocs.horse/#topic-message
 * Parameters: <channel> [<topic>] */
void Server::_topic(PollfdIterator* it, const std::vector<std::string>& args)
{
	Client& client = m_clients.find((*it)->fd)->second;
	if (args.size() < 2)
		return client.send_461("TOPIC");

	const std::string& channel_name = args[1];
	std::map<std::string, Channel>::iterator target_channel_it = m_channels.find(channel_name);
	if (target_channel_it == m_channels.end())
		return client.send_403(channel_name);

	Channel& channel = target_channel_it->second;
	if (args.size() == 2)
	{
		if (channel.topic == "")
			return client.send_331(channel);
		client.send_332(channel);
		client.send_333(channel);
		return;
	}

	if (!channel.is_subscribed(client.fd))
		return client.send_442(channel);

	if (channel.is_const_topic_mode && !channel.is_operator(client.fd))
		return client.send_482(channel);

	channel.topic = args[2];
	channel.topic_timestampt = long_to_str(time(NULL));
	channel.topic_starter_nickname = client.nickname;
	channel.send_msg(_topic_message(client, channel));
}

static std::string _topic_message(const Client& client, const Channel& channel)
{
	return client.nickname + " TOPIC " + channel.name + " :" + channel.topic;
}
