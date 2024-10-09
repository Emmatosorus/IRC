#include "../../include/Server.hpp"
#include <iostream>

/* https://modern.ircdocs.horse/#pass-message
 * Parameters: <password> */
void Server::_pass(PollfdIterator it, const std::vector<std::string>& args)
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
	// TODO: check if the client is already registered
	// TODO: validate password
	const std::string& password = args[1];
	(void)password;

	// this is how we can find client in the map and check if he is registered
	std::map<int, Client>::iterator connected_client = m_clients.find(it->fd);
	if (connected_client->second.is_registered)
	{
		// ERR_ALREADYREGISTERED (462)
	}
	else
	{
		// if password is correct, bla bla bla
		// else ERR_PASSWDMISMATCH (464) 
	}
	// TODO: send appropriate numeric replies
    /* ERRORS: 
	 * ERR_NEEDMOREPARAMS (461)
	 * ERR_ALREADYREGISTERED (462)
	 * ERR_PASSWDMISMATCH (464)
	 * */
}
