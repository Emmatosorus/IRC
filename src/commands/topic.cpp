#include "../../include/Server.hpp"
#include <iostream>

/* https://modern.ircdocs.horse/#topic-message
 * Parameters: <channel> [<topic>] */
void Server::_topic(PollfdIterator it, const std::vector<std::string>& args)
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
	std::map<int, Client>::iterator topic_starter = m_clients.find(it->fd);
	if (target_channel != m_channels.end())
	{
		// ERR_NOSUCHCHANNEL (403)
		return;
	}
	if (target_channel->second.is_const_topic_mode)
	{
		// find user who tries to change topic, check his privilege
		if (!topic_starter->second.is_operator)
		{
			// ERR_CHANOPRIVSNEEDED (482)
			return;
		}
		// user is operator,handle changing of topic
		return;
	}
	// not in const topic mode, handle changing of topic

	/* ERRORS:
	 * ERR_NEEDMOREPARAMS (461)
	 * ERR_NOSUCHCHANNEL (403)
	 * ERR_NOTONCHANNEL (442)
	 * ERR_CHANOPRIVSNEEDED (482)
     *
	 * REPLIES:
	 * RPL_NOTOPIC (331)
	 * RPL_TOPIC (332)
	 * RPL_TOPICWHOTIME (333) */
}
