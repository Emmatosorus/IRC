#ifndef SERVER_HPP
#define SERVER_HPP
#include "Channel.hpp"
#include "Client.hpp"
#include <map>
#include <string>
#include <vector>
#include <sys/poll.h>
#define MAX_CONNECTIONS 16
#define MESSAGE_SIZE 512
#define POLL_TIMEOUT 10000
class Server
{
	public:
	Server(const char* password, const char* port);
	Server(const Server&) = delete;
	Server& operator=(const Server&) = delete;
	~Server();

	void start();

	private:
	void _user(const std::vector<std::string>& args);
	void _pass(const std::vector<std::string>& args);
	void _nick(const std::vector<std::string>& args);
	void _join(const std::vector<std::string>& args);
	void _privmsg(const std::vector<std::string>& args);
	void _list(const std::vector<std::string>& args);
	void _names(const std::vector<std::string>& args);
	void _notice(const std::vector<std::string>& args);
	void _quit(const std::vector<std::string>& args);
	void _ping(const std::vector<std::string>& args);

	const std::string m_password;
	const std::string m_port;
	std::vector<struct pollfd> m_pfds;
	std::map<int, Client> m_clients;
	std::map<std::string, Channel> m_channels;
	std::map<std::string, void (Server::*)(const std::vector<std::string>&)> m_commands;
	int m_sockfd;
};
#endif
