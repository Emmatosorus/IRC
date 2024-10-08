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
	// Server(const Server&);
	// Server& operator=(const Server&);
	~Server();

	void start();

	private:
	typedef std::vector<struct pollfd>::iterator PollfdIterator;
	void _init_listening_socket();
	void _remove_client(int fd);
	void _remove_client(PollfdIterator* it);
	void _add_client(int fd);

	void _user(PollfdIterator it, const std::vector<std::string>& args);
	void _pass(PollfdIterator it, const std::vector<std::string>& args);
	void _nick(PollfdIterator it, const std::vector<std::string>& args);
	void _join(PollfdIterator it, const std::vector<std::string>& args);
	void _privmsg(PollfdIterator it, const std::vector<std::string>& args);
	void _list(PollfdIterator it, const std::vector<std::string>& args);
	void _names(PollfdIterator it, const std::vector<std::string>& args);
	void _notice(PollfdIterator it, const std::vector<std::string>& args);
	void _quit(PollfdIterator it, const std::vector<std::string>& args);
	void _ping(PollfdIterator it, const std::vector<std::string>& args);

	const std::string m_password;
	const std::string m_port;
	std::vector<struct pollfd> m_pfds;
	std::map<int, Client> m_clients;
	std::map<std::string, Channel> m_channels;
	std::map<std::string, void (Server::*)(PollfdIterator, const std::vector<std::string>&)> m_commands;
	int m_sockfd;
};
#endif
