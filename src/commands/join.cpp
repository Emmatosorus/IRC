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
		// TODO: do channel creation
	}
	else
	{
		// TODO: do channel joining logic
	}
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
