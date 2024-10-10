/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:34:22 by eandre            #+#    #+#             */
/*   Updated: 2024/10/10 20:50:19 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/bot.hpp"
#include <fstream>
#include <cstdlib>
#define MAXDATASIZE 200

typedef struct banned_words
{
	std::string	channel;
	std::vector<std::string>	words;
}	banned_words;

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int connect_with_timeout(int socket_fd, const struct sockaddr *addr, socklen_t addrlen, int timeout_sec)
{
	int	so_error;
	fd_set	writefds;
	socklen_t	len = sizeof(int);
	int	select_rv;
	struct timeval	timeout = {timeout_sec, 0};

	fcntl(socket_fd, F_SETFL, O_NONBLOCK);
	if (connect(socket_fd, addr, addrlen) == 0)
		return (0);
	if (errno != EINPROGRESS)
		return (-1);
	FD_ZERO(&writefds);
	FD_SET(socket_fd, &writefds);
	select_rv = select(socket_fd + 1, NULL, &writefds, NULL, &timeout);
	if (select_rv > 0 && FD_ISSET(socket_fd, &writefds))
	{
		getsockopt(socket_fd, SOL_SOCKET, SO_ERROR, &so_error, &len);
		if (so_error == 0)
			return (0);
	}
	return (-1);
}

void	set_addrinfo(struct addrinfo *ai)
{
	ai->ai_addr = NULL;
	ai->ai_addrlen = 0;
	ai->ai_canonname = NULL;
	ai->ai_addr = NULL;
	ai->ai_flags = 0;
	ai->ai_protocol = 0;
	ai->ai_family = AF_UNSPEC;
	ai->ai_socktype = SOCK_STREAM;
}

int	get_channel_from_pm(std::string arg, std::string *channel)
{
	size_t	pos = 0;
	std::string	command;

	pos = arg.find(":", 1);
	if (pos != std::string::npos)
	{
		if (!arg[pos + 1])
			return (-1);
		command = &arg[pos + 1];
		pos = command.find("!botjoin ", 0);
		if (pos != 0)
			return (-1);
		*channel = &command[9];
	}
	else
		return (-1);
	return (0);
}

bool	str_is_space(std::string *str)
{
	bool	rv;

	rv = true;
	for(std::string::iterator i = str->begin(); i != str->end(); i++)
		if (!((*i > 9 && *i < 13) || *i == ' '))
			rv = false;
	return (rv);
}

int	fill_slurs_vector(std::vector<std::string>	*slurs)
{
	std::ifstream	infile("slurs.txt");
	std::string		test;
	if (!infile)
		return (1);
	if (infile.is_open())
		while (std::getline(infile, test))
			if (str_is_space(&test) == false)
				slurs->push_back(test);
	return (0);
}

//
//guardian
bool	pm_is_in_channel(std::string msg)
{
	size_t	pos = 0;
	size_t	pos2 = 0;
	pos2 = msg.find("PRIVMSG", 0);
	if (pos2 != std::string::npos)
	{
		pos = msg.find("#", 0);
		if (pos != std::string::npos)
		{
			pos2 = msg.find(":", 0);
			if (pos2 < pos)
				return (false);
		}
	}
	return (true);
}

int	main(int argc, char **argv)
{
	struct pollfd	pollfds[1];
	std::vector<banned_words>	banned_words;
	int	socket_fd;
	struct addrinfo hints;
	struct addrinfo *tmp;
	struct addrinfo *servinfo;

	if (argc != 2)
	{
		std::cout << "\033[0;31mUsage : ./bot <IP addr of server> <Server password>\033[0m" << std::endl;
		return (1);
	}

	// if (fill_slurs_vector(&slurs) == 1)
	// 	return (3);
	set_addrinfo(&hints);
	if (getaddrinfo(argv[1], MYPORT, &hints, &servinfo) != 0)
		return (1);
	for (tmp = servinfo; tmp != NULL; tmp = tmp->ai_next)
	{
		socket_fd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
		if (socket_fd == -1)
		{
			std::cout << "\033[0;31mError! Socket error!\033[0m" << std::endl;
			continue;
		}
		if (connect_with_timeout(socket_fd, tmp->ai_addr, tmp->ai_addrlen, 1) == -1)
		{
			std::cout << "\033[0;31mError! Connect error!\033[0m" << std::endl;
			close(socket_fd);
			continue;
		}
		break ;
	}
	if (tmp == NULL)
	{
		std::cout << "\033[0;31mError! The adress is not available\033[0m" << std::endl;
		freeaddrinfo(servinfo);
		return (1);
	}
	char s[INET6_ADDRSTRLEN];
	inet_ntop(tmp->ai_family, get_in_addr((struct sockaddr *)tmp->ai_addr),s, sizeof(s));
	freeaddrinfo(servinfo);
	int	num_bytes;
	char	buf[MAXDATASIZE];
	pollfds[0].fd = socket_fd;
	pollfds[0].events = POLLIN;

	// for (std::vector<std::string>::iterator it = slurs.begin(); it != slurs.end(); it++)
	// {
	// 	std::cout << "Slur : " << *it << "\n";
	// }
	poll(pollfds, 1, -1);
	num_bytes = recv(socket_fd, buf, MAXDATASIZE-1, 0);
	if (num_bytes == -1)
		return (1);
	buf[num_bytes] = '\0';
	std::cout << "bot received: " << buf << std::endl;
	std::string	msg;
	std::string	channel;
	msg = buf;
	if (pm_is_in_channel(msg) == false)
	{
		if (get_channel_from_pm(msg, &channel) == -1)
		{
			close(socket_fd);
			return (-1);
		}
		std::cout << channel << std::endl;
		msg = "JOIN ";
		msg.append(channel);
		msg.append("\r\n");
		send(socket_fd, msg.c_str(), msg.length(), 0);
		close(socket_fd);
		return (0);
	}
	else
	{
		close(socket_fd);
		return (0);
	}
	// return (1);
	// size_t	pos = 0;
	// size_t	pos2 = 0;
	// pos = msg.find("#", 0);
	// if (pos != std::string::npos)
	// 	pos2 = msg.find(" ", pos);
	// msg[pos2] = '\0';
	// // std::cout << pos << std::endl;
	// std::string msg2 = "PRIVMSG @%#";
	// std::string channel;
	// if (pos == std::string::npos)
	// {
	// 	channel = &msg[msg.length()];
	// 	// std::cout << " Missing # (this is not supposed to happen in normal situations)" << std::endl;
	// }
	// else
	// 	channel = &msg[pos + 1];
	// msg2.append(channel);
	// msg2.append(" : MY GOD NO DON'T DO THAT\r\n");
	// // for (std::vector<std::string>::iterator it = slurs.begin(); it != slurs.end(); it++)
	// // {
	// 	if (msg.find("tranny", 0) != std::string::npos)
	// 		send(socket_fd, msg2.c_str(), msg2.length(), 0);
	// // }
	close(socket_fd);
	return (200);
}
