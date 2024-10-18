/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 18:19:04 by eandre            #+#    #+#             */
/*   Updated: 2024/10/18 14:10:00 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/bot.hpp"
#include "../../../include/ai.hpp"
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#define MAXDATASIZE 500

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

int	get_socket_fd(char **argv)
{
	int				socket_fd;
	struct addrinfo	hints;
	struct addrinfo	*tmp;
	struct addrinfo	*servinfo;

	set_addrinfo(&hints);
	if (getaddrinfo(argv[1], argv[2], &hints, &servinfo) != 0)
	{
		std::cout << "\033[0;31mError! Getaddrinfo error\033[0m" << std::endl;
		return (-1);
	}
	for (tmp = servinfo; tmp != NULL; tmp = tmp->ai_next)
	{
		socket_fd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
		if (socket_fd == -1)
		{
			std::cout << "\033[0;31mError! Socket error\033[0m" << std::endl;
			continue;
		}
		if (connect_with_timeout(socket_fd, tmp->ai_addr, tmp->ai_addrlen, 1) == -1)
		{
			std::cout << "\033[0;31mError! Connect error\033[0m" << std::endl;
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

bool	is_name_incorrect(std::string msg, std::string bot_name)
{
	return (msg == ":42chan 432 guest " + bot_name + " :Invalid nickname: Nickname contains invalid characters : #:,*?!@.\\t\\r\\n \r\n"
		|| msg == ":42chan 432 guest " + bot_name + " :Invalid nickname: First character of nickname must be a-z or A-Z\r\n"
		|| msg  == ":42chan 433 guest " + bot_name + " :Nickname is already taken\r\n"
		|| msg == ":42chan 431 guest :No nickname was given\r\n"
		|| msg == ":42chan 461 " + bot_name + " USER :Not enough prameters\r\n"
		|| msg == ":42chan 468 " + bot_name + " :Invalid username: username contains invalid characters: #:,*?!@.\\t\\r\\n \r\n"
		|| msg == ":42chan 417 guest :Message is too long\r\n"
		|| msg == ":42chan 468 " + bot_name + " :Invalid username: username is too long\r\n");
}

int	connect_to_server(int socket_fd, std::string password, std::string bot_name)
{
	std::string	msg;

	msg = "PASS " + password + "\r\n";
	std::cout << "bot send3: " << msg << std::endl;
	if (send(socket_fd, msg.c_str(), msg.length(), 0) == -1)
	{
		std::cout << "\033[0;31mError! Send error\033[0m" << std::endl;
		return (close(socket_fd), 1);
	}
	msg.clear();
	usleep(10);
	msg = "NICK " + bot_name + "\r\n";
	if (send(socket_fd, msg.c_str(), msg.length(), 0) == -1)
	{
		std::cout << "\033[0;31mError! Send error\033[0m" << std::endl;
		return (close(socket_fd), 1);
	}
	msg.clear();
	usleep(10);
	msg = "USER " + bot_name + " * *:" + bot_name + "\r\n";
	if (send(socket_fd, msg.c_str(), msg.length(), 0) == -1)
	{
		std::cout << "\033[0;31mError! Send error\033[0m" << std::endl;
		return (close(socket_fd), 1);
	}
	return (0);
}

std::string	get_sender_name(std::string msg)
{
	std::string sender_name;
	std::string tmp;
	size_t		pos = 0;

	pos = msg.find(" PRIVMSG", 1);
	tmp = msg;
	tmp[pos] = '\0';
	tmp = &tmp[1];
	sender_name = tmp;
	return (sender_name);
}

int	main(int argc, char **argv)
{
	struct pollfd	pollfds[1];
	std::string	bot_name = "NyanBot";
	std::string password;
	int	socket_fd;

	if (argc != 4 && argc != 5)
	{
		std::cout << "\033[0;31mUsage : ./bot <IP addr of server> <Port of server> <Server password> <Name (optionnal)>\033[0m" << std::endl;
		return (1);
	}
	if (argc == 5)
		bot_name = argv[4];
	password = argv[3];
	socket_fd = get_socket_fd(argv);
	if (socket_fd == -1)
		return (1);
	//the mess begins here

	int	num_bytes;
	pollfds[0].fd = socket_fd;
	pollfds[0].events = POLLIN;
	std::string	msg;
	std::string	sender_name;
	std::string		get;
	std::string	curl_cmd;
	int		step = 0;
	char	buf[MAXDATASIZE];
	while (42)
	{
		if (step != 0)
		{
			if (poll(pollfds, 1, -1) == -1)
			{
				std::cout << "\033[0;31mError! Poll error\033[0m" << std::endl;
				return (close(socket_fd), 1);
			}
			num_bytes = recv(socket_fd, buf, MAXDATASIZE-1, 0);
			if (num_bytes == -1)
			{
				std::cout << "\033[0;31mError! Recv error\033[0m" << std::endl;
				return (close(socket_fd), 1);
			}
			if (num_bytes == 0)
			{
				std::cout << "\033[0;31mError! The server closed\033[0m" << std::endl;
				return (close(socket_fd), 1);
			}
			buf[num_bytes] = '\0';
			msg = buf;
		}
		switch (step)
		{
			case 0:
			{
				if (connect_to_server(socket_fd, password, bot_name) == 1)
					return (1);
				step++;
				break ;
			}
			case 1:
			{
				std::cout << "bot received: " << buf;
				if (msg == ((":42chan 464 ") + bot_name + (" :Password is incorrect or was not provided\r\n")) || msg == ":42chan 461 guest PASS :Not enough prameters\r\n:42chan 464 guest :Password is incorrect or was not provided\r\n")
				{
					std::cout << "\033[0;31mError! Incorrect password\033[0m" << std::endl;
					return (close(socket_fd), 1);
				}
				if (is_name_incorrect(msg, bot_name))
				{
					std::cout << "\033[0;31mError! Incorrect name\033[0m" << std::endl;
					return (close(socket_fd), 1);
				}
				if (msg != ":42Chan 001 " + bot_name + " :Welcome to the 42Chan Network " + bot_name + "!\r\n")
				{
					std::cout << "\033[0;31mError! This bot is restricted to our irc only!\033[0m" << std::endl;
					return (close(socket_fd), 1);
				}
				step++;
				break ;
			}
			case 2:
			{
				curl_cmd = CURL_CMD;
				size_t	pos = 0;
				size_t	pos2 = 0;
				size_t	pos3 = 0;
				if (msg.find("PRIVMSG", 0) == std::string::npos)
					continue ;
				sender_name = get_sender_name(msg);
				// pos = curl_cmd.find("content", 0);
				// if (pos == std::string::npos)
				// 	continue ;
				pos = curl_cmd.find("content", 600);
				if (pos == std::string::npos)
					continue ;
				pos += 11;
				pos2 = curl_cmd.find("\"", pos);
				if (pos2 == std::string::npos)
					continue ;
				pos3 = msg.find("\r\n", 0);
				msg.erase(pos3, 2);
				pos3 = msg.find_first_of("\'\"", 0);
				while (pos3 != std::string::npos && pos2 > pos3)
				{
					msg.erase(pos3, 1);
					msg.insert(pos3, " ");
					pos3 = msg.find_first_of("\'\"", pos3 + 1);
				}
				curl_cmd.erase(pos, pos2 - pos);
				curl_cmd.insert(pos, msg);
				std::cout << "bot cmd:"<< curl_cmd << std::endl;
				FILE *fp = popen(curl_cmd.c_str(), "r");
				if (fp == NULL)
					return (close(socket_fd), 1);
				char	buff[1024];
				
				while (fgets(buff, 1024, fp) != NULL)
				{
					get = buff;
					pos = get.find("content", 0);
					if (pos != std::string::npos)
						break ;
				}
				int status = pclose(fp);
				if (status == -1)
					return (close(socket_fd), 1);
				if (pos == std::string::npos)
					continue ;
				pos += 11;
				pos2 = get.find("\"", pos);
				pos3 = pos2;
				while (pos3 != std::string::npos)
				{
					pos2 = get.find("\"", pos3 + 1);
					pos3 = get.find("\"", pos2 + 1);
				}
				if (pos2 == std::string::npos)
					continue ;
				get[pos2] = '\0';
				get = &get[pos];
				msg = "PRIVMSG " + sender_name + ": " + get;
				msg.append("\r\n");
				std::cout << msg << std::endl;
				send(socket_fd, msg.c_str(), msg.length(), 0);
				break ;
			}
		}
		buf[0] = '\0';
		msg.clear();
		get.clear();
		sender_name.clear();
		curl_cmd.clear();
	}
	close(socket_fd);
}