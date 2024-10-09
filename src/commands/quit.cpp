#include "../../include/Server.hpp"
#include <iostream>

/* https://modern.ircdocs.horse/#quit-message
 * Parameters: [<reason>] */
void Server::_quit(PollfdIterator it, const std::vector<std::string>& args)
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
	if (args.size() > 1)
	{
		// distribute quit message
	}
	// this is how to remove client
	_remove_client(it->fd);
	// no numeric replies
}
