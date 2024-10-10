#include "../../include/Server.hpp"
#include <iostream>

static int check_user_args(std::vector<struct pollfd>::iterator it, const std::vector<std::string>& args);

/* https://modern.ircdocs.horse/#user-message
 * Parameters: <username> 0 * <realname> */
void Server::_user(PollfdIterator it, const std::vector<std::string>& args)
{
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

	if (check_user_args(it, args) != 0)
		return ;
	// TODO: validate username according to the spec
	// check if the user is registered already or not
	std::map<int, Client>::iterator client = m_clients.find(it->fd);
	if (client->second.is_registered)
	{
		// TODO: handle registration
		// ERR_ALREADYREGISTERED (462)
		return;
	}
	if (!client->second.entered_password)
	{
		// TODO: Send message to client
		// User must enter password first
		return;
	}
	Client	new_client(it->fd);
	new_client.username = args[1];
	new_client.fullname = args[2];
	m_clients.insert(std::make_pair(it->fd, new_client));
}

static int check_user_args(std::vector<struct pollfd>::iterator it, const std::vector<std::string>& args)
{
	if (args.size() != 3)
	{
		// TODO: handle registration
		// ERR_NEEDMOREPARAMS (461)
		return (1);
	}
	size_t pos = args[1].find("#:,");
	if (pos != std::string::npos)
	{
		// TODO: send message to client
		// bad arguments for user
		return (1);
	}
	return (0);
}