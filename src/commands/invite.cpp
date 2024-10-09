#include "../../include/Server.hpp"
#include <iostream>

/* https://modern.ircdocs.horse/#invite-message
 * Parameters: <nickname> <channel> */
void Server::_invite(PollfdIterator it, const std::vector<std::string>& args)
{
	(void)args;
	(void)it;
	(void)args;
	(void)it;
	for (size_t i = 0; i < args.size(); i++)
	{
		std::cout << args[i];
		if (i != args.size() - 1)
			std::cout << ' ';
	}
	std::cout << '\n';
	// TODO: validated arguments
	// =====
	const std::string& channel_name = args[1];
	std::map<std::string, Channel>::iterator target_channel = m_channels.find(channel_name);
	if (target_channel != m_channels.end())
	{
		// TODO: do channel creation
	}
	else
	{
		// TODO: do channel joining logic
	}
	// TODO: send appropriate numeric replies
    /* ERRORS:
	 * ERR_NEEDMOREPARAMS (461)
	 * ERR_NOSUCHCHANNEL (403)
	 * ERR_NOTONCHANNEL (442)
	 * ERR_CHANOPRIVSNEEDED (482)
	 * ERR_USERONCHANNEL (443)
     *
	 * REPLIES:
	 * RPL_INVITING (341)
	 * */
}
