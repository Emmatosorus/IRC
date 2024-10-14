#include "../../include/Server.hpp"
#include <iostream>

/* https://modern.ircdocs.horse/#user-message
 * Parameters: <username> : <realname> */
void Server::_user(PollfdIterator it, const std::vector<std::string>& args)
{
	if (_check_user_args(it, args) != 0)
		return ;
	std::map<int, Client>::iterator client = m_clients.find(it->fd);
	if (!client->second.entered_password)
	{
		_send_to_client(it->fd, "1003", "You must first enter password");
		return;
	}
	if (client->second.username != "")
	{
		_send_to_client(it->fd, "1004", "Username already set");
		return ;
	}
	if (!client->second.is_registered)
	{
		client->second.username = args[1];
		client->second.fullname = args[2];
		if (client->second.nickname != "")
		{
			if (client->second.password != this->m_password)
			{
				_send_to_client(it->fd, "464", "Incorrect password");
				m_clients.erase(client);
				return ;
			}
			client->second.is_registered = true;
			_send_to_client(it->fd, "001", "Welcome to 42Chan Network!");
		}
		return ;
	}
	else
	{
		_send_to_client(it->fd, "462", "You are already registered");
		return;
	}
}

int Server::_check_user_args(PollfdIterator it, const std::vector<std::string>& args)
{
	if (args.size() < 3)
	{
		Server::_send_to_client(it->fd, "461", "Not enough parameters :\nUSER <username> : <realname>");
		return (1);
	}
	if (args.size() > 3)
	{
		Server::_send_to_client(it->fd, "461", "Too many parameters :\nUSER <username> : <realname>");
		return (1);
	}
	if (args[1].size() > USERLEN)
	{
		Server::_send_to_client(it->fd, "1001", "Username has more than 18 characters");
		return (1);
	}
	size_t pos = args[1].find_first_of("#:,*?!@.\t\r\n ");
	if (pos != std::string::npos)
	{
		Server::_send_to_client(it->fd, "1002", "Username contains invalid characters : #:,*?!@.\\t\\r\\n ");
		return (1);
	}
	return (0);
}