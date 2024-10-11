#include "../../include/Server.hpp"
#include <iostream>

/* https://modern.ircdocs.horse/#join-message
 * Parameters: <channel>{,<channel>} [<key>{,<key>}] */
void Server::_join(PollfdIterator it, const std::vector<std::string>& args)
{
	(void)args;
	(void)it;
	for (size_t i = 0; i < args.size(); i++)
	{
		std::cout << args[i];
		if (i != args.size() - 1)
			std::cout << ' ';
	}
	std::cout << '\n';
	// TODO: validate and parse arguments
	// =====
	const std::string& channel_name = args[1];
	std::map<std::string, Channel>::iterator target_channel = m_channels.find(channel_name);
	if (target_channel != m_channels.end())
	{
		// TODO: do channel creation, the user which created the channel joins it, becomes it creator, operator etc
		Channel new_channel (it->fd, channel_name);
		m_channels.insert(make_pair(channel_name, new_channel));
		return;
	}
	Channel& channel = target_channel->second;
	// TODO: check password
	if  (channel.password_mode.first)
	{
		// TODO: validate password
	}
	// TODO: do channel joining logic
	ClientIterator client_it = m_clients.find(it->fd);
	if (client_it == m_clients.end())
		return;
	Client& client = m_clients.find(it->fd)->second;
	channel.subscribed_users_fd.push_back(it->fd);
	std::string message_to_send = ":" + client.nickname + " JOIN " + channel_name;
	std::vector<int>::iterator fd_it;
	for (fd_it = channel.subscribed_users_fd.begin(); fd_it != channel.subscribed_users_fd.end(); fd_it++)
	{
		if (*fd_it != it->fd)
			_send_to_client(*fd_it, "", message_to_send);
	}
	if (channel.topic.topic != "")
		message_to_send += "\n" + client.nickname + " " + channel_name + " " + ":" + channel.topic.topic;
	else
		message_to_send += "\n" + client.nickname + " " + channel_name + " " + ":No topic is set";

	// TODO: send appropriate numeric replies
    /* ERRORS:
	 * ERR_NEEDMOREPARAMS (461)
     * ERR_BADCHANNELKEY (475)
     * ERR_BANNEDFROMCHAN (474)
     * ERR_CHANNELISFULL (471)
     * ERR_INVITEONLYCHAN (473)
	 *
	 * REPLIES:
     * RPL_TOPIC (332)
     * RPL_TOPICWHOTIME (333)
     * RPL_NAMREPLY (353)
     * RPL_ENDOFNAMES (366)
	 * */
}
