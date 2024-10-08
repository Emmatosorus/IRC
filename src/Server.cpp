#include "../include/Server.hpp"
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <netdb.h>
#include <unistd.h>
#include <stdexcept>

Server::Server(const char* password, const char* port) : m_password(password), m_port(port), m_pfds(), m_sockfd(-1)
{
	struct addrinfo* ai;
	struct addrinfo hints = {};

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	int gai_status = getaddrinfo(NULL, port, &hints, &ai);
	if (gai_status != 0)
	{
		throw std::runtime_error("Server: getaddrinfo failed");
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
		throw std::runtime_error("Server: socket binding failed");
	}

	if (listen(m_sockfd, MAX_CONNECTIONS) != 0)
	{
		close(m_sockfd);
		throw std::runtime_error("Server: listen failed");
	}

	m_pfds.reserve(MAX_CONNECTIONS);
	struct pollfd serv_pfd;
	serv_pfd.fd = m_sockfd;
	serv_pfd.events = POLLIN;
	m_pfds.push_back(serv_pfd);

	// messages are case insensitive, hence commands are lower case
	m_commands["join"] = &Server::_join;
	m_commands["names"] = &Server::_names;
	m_commands["nick"] = &Server::_nick;
	m_commands["notice"] = &Server::_notice;
	m_commands["pass"] = &Server::_pass;
	m_commands["ping"] = &Server::_ping;
	m_commands["privmsg"] = &Server::_privmsg;
	m_commands["quit"] = &Server::_quit;
	m_commands["user"] = &Server::_user;
}

Server::~Server()
{
	close(m_sockfd);
	for (size_t i = 0; i < m_pfds.size(); i++)
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
			throw std::runtime_error("Server: poll failed");
		}
		
		for (size_t i = 0; i < m_pfds.size(); i++)
		{
			// if it's server's fd
			if ((m_pfds[i].revents & POLLIN) && m_pfds[i].fd == m_sockfd)
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

				struct pollfd new_pollfd;
				new_pollfd.fd = new_fd;
				new_pollfd.events = POLLIN;
				m_clients.emplace(new_fd, Client(new_fd));
				m_pfds.push_back(new_pollfd);
				std::cout << "New connection: " << new_pollfd.fd << "\n";
			}
			// if it's client's fd
			else if ((m_pfds[i].revents & POLLIN) && m_pfds[i].fd != m_sockfd)
			{
				int bytes_received = recv(m_pfds[i].fd, buf, MESSAGE_SIZE, 0);
				if (bytes_received < 0)
				{
					std::cerr << "Server: recv failed\n";
					continue;
				}
				// the client is disconnected
				else if (bytes_received == 0)
				{
					std::cout << "Connection closed: " << m_pfds[i].fd << "\n";
					close(m_pfds[i].fd);
					m_clients.erase((m_pfds.begin() + i)->fd);
					m_pfds.erase(m_pfds.begin() + i);
					continue;
				}
				// TODO: do the proper parsing of the buffer				
				buf[bytes_received] = '\0';
				std::string msg = buf;
				std::cout << msg << '\n';
				if (m_commands.find(msg) != m_commands.end())
				{
					std::vector<std::string> dummy_args(3, "");
					(this->*m_commands[msg])(dummy_args);
				}
				else
				{
					// TODO: send numeric reply of an unknown command
				}
			}
		}
	}
}
