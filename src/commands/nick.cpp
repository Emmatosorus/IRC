#include "../../include/Server.hpp"
#include <iostream>

/* https://modern.ircdocs.horse/#nick-message
 * Parameters: <nickname> */
void Server::_nick(PollfdIterator it, const std::vector<std::string>& args)
{
	if (_check_nick_args(it, args) != 0)
		return ;
	const std::string& nickname = args[1];
	ClientIterator potential_nickname_holder = _find_client_by_nickname(nickname);
	if (potential_nickname_holder != m_clients.end())
	{
		potential_nickname_holder->second.nickname = nickname;
		return;
	}
	std::map<int, Client>::iterator client = m_clients.find(it->fd);
	if (!client->second.entered_password)
	{
		_send_to_client(it, "1003", "You must first enter password");
		return;
	}
	client->second.nickname = nickname;
	if (client->second.username != "")
	{
		if (client->second.password != this->m_password)
		{
			_send_to_client(it, "464", "Incorrect password");
			return ;
		}
		if (!client->second.is_registered)
		{
			client->second.is_registered = true;
			_send_to_client(it, "001", "Welcome to 42Chan Network!");
		}
	}
}

int Server::_check_nick_args(PollfdIterator it, const std::vector<std::string>& args)
{
	(void)it;
	if (args.size() < 2)
	{
		Server::_send_to_client(it, "431", "No nickname was given :\nNICK <nickname>");
		return (1);
	}
	if (args.size() > 2)
	{
		Server::_send_to_client(it, "461", "Too many parameters :\nNICK <nickname>");
		return (1);
	}
	if (std::isalpha(args[1][0]) == 0)
	{
		Server::_send_to_client(it, "1002", "First character of nickname must be a-z or A-Z");
		return (1);
	}
	size_t pos = args[1].find_first_of("#:,*?!@.\t\r\n ");
	if (pos != std::string::npos)
	{
		Server::_send_to_client(it, "1002", "Nickname contains invalid characters : #:,*?!@.\\t\\r\\n ");
		return (1);
	}
	Server::ClientIterator client_it = _find_client_by_nickname(args[1]);
	if (client_it != m_clients.end() && client_it->second.fd != it->fd)
	{
		Server::_send_to_client(it, "433", "Nickname is already taken");
		return (1);
	}
	return (0);
}