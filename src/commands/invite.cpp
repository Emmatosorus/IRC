#include "../../include/Server.hpp"
#include <iostream>

/* https://modern.ircdocs.horse/#invite-message
 * Parameters: <nickname> <channel> */
void Server::_invite(PollfdIterator it, const std::vector<std::string>& args)
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
	const std::string& channel_name = args[2];
	std::map<std::string, Channel>::iterator target_channel = m_channels.find(channel_name);
	if (target_channel != m_channels.end())
	{
		// ERR_NOSUCHCHANNEL (403)
		return;
	}
	// TODO: do channel joining logic, a lot of stuff should be checked, like:
	// 1. does user exist
	// 2. is inviting user present on the channel
	// 3. is invited user present on the channel
	// 4. is channel invite only
	//    - if it is, check if the inviter is an operator

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
