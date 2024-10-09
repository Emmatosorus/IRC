#include "../../include/Server.hpp"
#include <iostream>

/* https://modern.ircdocs.horse/#ping-message
 * Parameters: <token> */
void Server::_ping(PollfdIterator it, const std::vector<std::string>& args)
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
	// TODO: validate arguments
	// =====
	// TODO: generate token and send it
	// atm I am not sure how to implement ping, but this command will be sent
	// by server after a specific timeout by the client
	// we also might want to skip doing this command and do PONG instead,
	// as the clients also have the capability to ping, and server will have to
	// handle it somehow
	// token can be anything. in this video
	// https://www.youtube.com/watch?v=FDrR98ww6bE
	// the token is simply a server name. we might do something like this
	// as well for the simplicity sake
	const std::string& token = args[1];
	(void)token;
	// TODO: send appropriate numeric replies
    /* ERRORS: 
     * ERR_NEEDMOREPARAMS (461)
     * ERR_NOORIGIN (409)
	 * */
}
