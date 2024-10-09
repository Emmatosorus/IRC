#include "../../include/Server.hpp"
#include <iostream>

/* https://modern.ircdocs.horse/#names-message
 * Parameters: <channel>{,<channel>} */
void Server::_names(PollfdIterator it, const std::vector<std::string>& args)
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
		// TODO: not sure tbh. send back empty list ???
	}
	else
	{
		// TODO: do listing of the nicknames on the network
	}
	// TODO: send appropriate numeric replies
    /* REPLIES: 
	 * RPL_NAMREPLY (353) 
	 * RPL_ENDOFNAMES (366)
	 * */
}
