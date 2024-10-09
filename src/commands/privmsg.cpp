#include "../../include/Server.hpp"
#include <iostream>

/* https://modern.ircdocs.horse/#privmsg-message
 * Parameters: <target>{,<target>} <text to be sent> */
void Server::_privmsg(PollfdIterator it, const std::vector<std::string>& args)
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
	// TODO: validate existance of the targets
	const std::string& target = args[1];

	// how to find if the channel exists
	std::map<std::string, Channel>::iterator target_channel = m_channels.find(target);
	if (target_channel != m_channels.end())
	{
		// fds of all channel users
		(void)target_channel->second.subscribed_users_fd;
		// ...
	}

	// how to find if the user exists
	ClientIterator target_user = _find_client_by_nickname(target);
	if (target_user != m_clients.end())
	{
		// ...
	}
	// TODO: send appropriate numeric replies
    /* if I understand correctly, the same errors and replies as for PRIVMSG
	 * ERRORS:
	 * ERR_NOSUCHNICK (401)
	 * ERR_CANNOTSENDTOCHAN (404)
	 * ERR_TOOMANYTARGETS (407)
	 * ERR_NORECIPIENT (411)
	 * ERR_NOTEXTTOSEND (412)
	 *
	 * REPLIES:
	 * RPL_AWAY (301)
	 * */
}
