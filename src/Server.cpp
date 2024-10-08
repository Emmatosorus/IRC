#include "../include/Server.hpp"
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <netdb.h>
#include <unistd.h>
#include <stdexcept>

Server::Server(const char* password, const char* port) : m_password(password), m_port(port), m_pfds(), m_sockfd(-1)
{
	_init_listening_socket();

	m_pfds.reserve(MAX_CONNECTIONS);
	struct pollfd serv_pfd;
	serv_pfd.fd = m_sockfd;
	serv_pfd.events = POLLIN;
	m_pfds.push_back(serv_pfd);

	/* Commands are case insensitive, hence they are lower case. */
	m_commands.insert(make_pair("join", &Server::_join));
	m_commands.insert(make_pair("names", &Server::_names));
	m_commands.insert(make_pair("nick", &Server::_nick));
	m_commands.insert(make_pair("notice", &Server::_notice));
	m_commands.insert(make_pair("pass", &Server::_pass));
	m_commands.insert(make_pair("ping", &Server::_ping));
	m_commands.insert(make_pair("privmsg", &Server::_privmsg));
	m_commands.insert(make_pair("quit", &Server::_quit));
	m_commands.insert(make_pair("user", &Server::_user));
}

Server::~Server()
{
	close(m_sockfd);
	for (size_t i = 1; i < m_pfds.size(); i++)
	{
		close(m_pfds[i].fd);
	}
}

void Server::start()
{
	char buf[MESSAGE_SIZE + 1];
	while (1)
	{
		int poll_res = poll(m_pfds.data(), m_pfds.size(), POLL_TIMEOUT);
		if (poll_res == -1)
		{
			throw std::runtime_error("Critical error: poll failed");
		}
		
		for (PollfdIterator it = m_pfds.begin(); it != m_pfds.end(); it++)
		{
			// if it's server's fd
			if ((it->revents & POLLIN) && it->fd == m_sockfd)
			{
				struct sockaddr_storage client_addr;
				socklen_t addrlen = sizeof(struct sockaddr_storage);
				int new_fd = accept(m_sockfd,
						(struct sockaddr *)&client_addr, &addrlen);

				if (new_fd == -1)
				{
					std::cerr << "Server: new connection failed\n";
					continue;
				}

				_add_client(new_fd);
				std::cout << "Connection accepted: " << new_fd << "\n";
			}
			// if it's client's fd
			else if ((it->revents & POLLIN) && it->fd != m_sockfd)
			{
				int bytes_received = recv(it->fd, buf, MESSAGE_SIZE, 0);
				if (bytes_received < 0)
				{
					std::cerr << "Error: recv failed\n";
					continue;
				}
				// the client is disconnected
				else if (bytes_received == 0)
				{
					std::cout << "Connection closed: " << it->fd << "\n";
					_remove_client(&it);
					continue;
				}
				// TODO: do the proper parsing of the buffer
				buf[bytes_received] = '\0';
				std::cout << "Message received\n" << buf;
				Client& client = m_clients[it->fd];
				client.buf += buf;
				if (client.buf.length() > MESSAGE_SIZE)
				{
					// TODO: send numeric reply ERR_INPUTTOOLONG (417)
					continue;
				}

				size_t end_of_msg = client.buf.find("\n");
				std::string parsed_string;
				if (end_of_msg != std::string::npos)
				{
					parsed_string = client.buf.substr(0, end_of_msg);
					std::cout << "parsed_string: " << parsed_string << "\n";
					client.buf = client.buf.substr(end_of_msg + 1, std::string::npos);
					std::cout << "new client buf: " << client.buf << "\n";
				}
				else
				{
					continue;
				}

				if (m_commands.find(client.buf) != m_commands.end())
				{
					std::vector<std::string> dummy_args(3, "");
					(this->*m_commands[client.buf])(it, dummy_args);
				}
				else
				{
					// TODO: send numeric reply ERR_UNKNOWNCOMMAND (421)
				}
			}
		}
	}
}

void Server::_init_listening_socket()
{
	struct addrinfo* ai;
	struct addrinfo hints = {};

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	int gai_status = getaddrinfo(NULL, m_port.c_str(), &hints, &ai);
	if (gai_status != 0)
	{
		throw std::runtime_error("Critical error: getaddrinfo failed");
	}
	struct addrinfo* curr;
	for (curr = ai; curr != NULL; curr = curr->ai_next)
	{
		m_sockfd = socket(curr->ai_family, ai->ai_socktype, ai->ai_protocol);
		if (m_sockfd < 0)
			continue;

		int yes = 1;
        if (setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) != 0)
		{
			close(m_sockfd);
			continue;
		}

		if (bind(m_sockfd, curr->ai_addr, curr->ai_addrlen) != 0)
		{
			close(m_sockfd);
			continue;
		}

		break;
	}
	freeaddrinfo(ai);
	if (curr == NULL)
	{
		throw std::runtime_error("Critical error: socket binding failed");
	}

	if (listen(m_sockfd, MAX_CONNECTIONS) != 0)
	{
		close(m_sockfd);
		throw std::runtime_error("Server: listen failed");
	}
}

void Server::_remove_client(PollfdIterator* it)
{
	PollfdIterator old_it = *it;
	close(old_it->fd);
	m_clients.erase(old_it->fd);
	*it = m_pfds.erase(old_it);
	std::advance(*it, -1);
}

void Server::_add_client(int fd)
{
	struct pollfd new_pollfd;
	new_pollfd.fd = fd;
	new_pollfd.events = POLLIN;
	m_clients.insert(std::make_pair(fd, Client(fd)));
	m_pfds.push_back(new_pollfd);
}
