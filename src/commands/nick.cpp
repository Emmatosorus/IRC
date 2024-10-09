#include "../../include/Server.hpp"
#include <iostream>

/* https://modern.ircdocs.horse/#nick-message
 * Parameters: <nickname> */
void Server::_nick(PollfdIterator it, const std::vector<std::string>& args)
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
	// TODO: validate nickname
	const std::string& nickname = args[1];
	ClientIterator potential_nickname_holder = _find_client_by_nickname(nickname);
	if (potential_nickname_holder != m_clients.end())
	{
		// TODO: nickname is free, handle changing of the nickname
		return;
	}
	// TODO: ERR_NICKNAMEINUSE (433)

	// TODO: send appropriate numeric replies
    /* ERRORS: 
	 * ERR_NONICKNAMEGIVEN (431)
	 * ERR_ERRONEUSNICKNAME (432)
	 * ERR_NICKNAMEINUSE (433)
	 * */
}
