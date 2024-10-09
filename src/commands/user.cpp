#include "../../include/Server.hpp"
#include <iostream>

/* https://modern.ircdocs.horse/#user-message
 * Parameters: <username> 0 * <realname> */
void Server::_user(PollfdIterator it, const std::vector<std::string>& args)
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
	// NOTE: PASS and USER both handle registration
	// there are is_registered and enetered_password fields on the client

	// TODO: validate username according to the spec
	// check if the user is registered already or not
	std::map<int, Client>::iterator client = m_clients.find(it->fd);
	if (client->second.is_registered)
	{
		// ERR_ALREADYREGISTERED (462)
		return;
	}
	// TODO: handle registration

	/* ERRORS:
     * ERR_NEEDMOREPARAMS (461)
     * ERR_ALREADYREGISTERED (462) */
}
