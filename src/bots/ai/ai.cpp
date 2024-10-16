/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 18:19:04 by eandre            #+#    #+#             */
/*   Updated: 2024/10/16 19:00:44 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/bot.hpp"
#include "../../../include/ai.hpp"
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#define MAXDATASIZE 500

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

int	get_socket_fd(char **argv)
{
	int				socket_fd;
	struct addrinfo	hints;
	struct addrinfo	*tmp;
	struct addrinfo	*servinfo;

	set_addrinfo(&hints);
	if (getaddrinfo(argv[1], argv[2], &hints, &servinfo) != 0)
	{
		std::cout << "\033[0;31mError! Getaddrinfo error!\033[0m" << std::endl;
		return (-1);
	}
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
		break;
	}
	if (tmp == NULL)
	{
		std::cout << "\033[0;31mError! The adress is not available\033[0m" << std::endl;
		freeaddrinfo(servinfo);
		return (-1);
	}
	freeaddrinfo(servinfo);
	return (socket_fd);
}

int	main(int argc, char **argv)
{
	struct pollfd	pollfds[1];
	int	socket_fd;

	if (argc != 4 && argc != 5)
	{
		std::cout << "\033[0;31mUsage : ./bot <IP addr of server> <Port of server> <Server password> <Name (optionnal)>\033[0m" << std::endl;
		return (1);
	}
	socket_fd = get_socket_fd(argv);
	if (socket_fd == -1)
		return (1);
	
	//the mess begins here

	int	num_bytes;
	pollfds[0].fd = socket_fd;
	pollfds[0].events = POLLIN;
	std::string	msg;
	// int		step = 0;
	char	buf[MAXDATASIZE];
	std::string password = argv[3];
	msg = "PASS " + password + "\r\n";
	std::cout << "bot send1: " << msg << std::endl;
	usleep(100);
	send(socket_fd, msg.c_str(), msg.length(), 0);
	msg.clear();
	msg = "USER Celiastral:Celia And\r\n";
	usleep(100);
	send(socket_fd, msg.c_str(), msg.length(), 0);
	msg.clear();
	msg = "NICK Celiastral\r\n";
	std::cout << "bot send3: " << msg << std::endl;
	usleep(100);
	send(socket_fd, msg.c_str(), msg.length(), 0);
	if (poll(pollfds, 1, -1) == -1)
		return (1);
	num_bytes = recv(socket_fd, buf, MAXDATASIZE-1, 0);
	if (num_bytes == -1)
		return (close(socket_fd), 1);
	msg = buf;
	if (msg != ":42Chan   :You are connected\n")
		return (-1);
	msg.clear();
	buf[num_bytes] = '\0';
	std::cout << "bot received 1: " << buf << std::endl;
	buf[0] ='\0';
	fcntl(socket_fd, F_SETFL, O_NONBLOCK);
	while (42)
	{
		std::string	channel;
		if (poll(pollfds, 1, -1) == -1)
			return (1);
		// switch (step)
		// {
		// 	case 0:
		// 		break ;
		// 	case 1:
		// 		break ;
		// 	case 2:
		// 		break ;
		// }
		num_bytes = recv(socket_fd, buf, MAXDATASIZE-1, 0);
		if (num_bytes == -1)
			return (close(socket_fd), 1);
		if (num_bytes == 0)
			return (close(socket_fd), 1);
		buf[num_bytes] = '\0';
		std::cout << "bot received: " << buf;
		msg = buf;
		if (msg == ":42Chan 464 Celiastral :Incorrect password\n")
			return (-1);
		std::string	curl_cmd(CURL_CMD);
		size_t	pos = 0;
		size_t	pos2 = 0;
		size_t	pos3 = 0;
		if (msg.find("PRIVMSG", 0) == std::string::npos)
			continue ;
		pos = curl_cmd.find("content", 0);
		if (pos == std::string::npos)
		{
			// close(socket_fd);
			continue ;
		}
		pos = curl_cmd.find("content", pos + sizeof("content"));
		if (pos == std::string::npos)
		{
			// close(socket_fd);
			continue ;
		}
		pos += 11;
		pos2 = curl_cmd.find("\"", pos);
		if (pos2 == std::string::npos)
		{
			// close(socket_fd);
			continue ;
		}
		// pos3 = pos2;
		// while (pos3 != std::string::npos)
		// {
		// 	pos2 = curl_cmd.find("\"", pos3 + 1);
		// 	pos3 = curl_cmd.find("\"", pos2 + 1);
		// }
		// curl_cmd[pos2] = '\0';
		pos3 = msg.find("\'", 0);
		while (pos3 != std::string::npos)
		{
			msg.erase(pos3, 1);
			msg.insert(pos3, " ");
			pos3 = msg.find("\'", pos3 + 1);
		}
		curl_cmd.erase(pos, pos2 - pos);
		curl_cmd.insert(pos, msg);
		std::cout << "bot cmd:"<< curl_cmd << std::endl;
		FILE *fp = popen(curl_cmd.c_str(), "r");
		if (fp == NULL)
			return (close(socket_fd), -1);
		char	buff[1024];
		std::string		get;
		while (fgets(buff, 1024, fp) != NULL)
		{
			get = buff;
			pos = get.find("content", 0);
			if (pos != std::string::npos)
				break ;
		}
		pclose(fp);
		if (pos == std::string::npos)
		{
			// close(socket_fd);
			continue ;
		}
		// close(socket_fd);
		pos += 11;
		pos2 = get.find("\"", pos);
		pos3 = pos2;
		while (pos3 != std::string::npos)
		{
			pos2 = get.find("\"", pos3 + 1);
			pos3 = get.find("\"", pos2 + 1);
		}
		if (pos2 == std::string::npos)
		{
			// close(socket_fd);
			continue ;
		}
		get[pos2] = '\0';
		get = &get[pos];
		msg = "PRIVMSG ";
		msg.append(get);
		msg.append("\r\n");
		std::cout << msg << std::endl;
		send(socket_fd, msg.c_str(), msg.length(), 0);
		buf[0] = '\0';
		msg.clear();
		get.clear();
		channel.clear();
		curl_cmd.clear();
	}
	close(socket_fd);
}