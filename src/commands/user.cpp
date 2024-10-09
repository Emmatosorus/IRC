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
	// TODO: validate username according to the spec
	// TODO: check if the user is registered already or not
	/* ERRORS:
     * ERR_NEEDMOREPARAMS (461)
     * ERR_ALREADYREGISTERED (462) */
}
