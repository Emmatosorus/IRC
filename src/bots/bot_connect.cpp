/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_connect.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:05:18 by eandre            #+#    #+#             */
/*   Updated: 2024/10/26 18:18:15 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/bot.hpp"
#include <fcntl.h>
#include <cerrno>

int	get_socket_fd(char **argv)
{
	int				socket_fd;
	struct addrinfo	hints;
	struct addrinfo	*tmp;
	struct addrinfo	*servinfo;

	set_addrinfo(&hints);
	if (getaddrinfo(argv[1], argv[2], &hints, &servinfo) != 0)
		return (error_msg("\033[0;31mError! Getaddrinfo error\033[0m", -1));
	for (tmp = servinfo; tmp != NULL; tmp = tmp->ai_next)
	{
		socket_fd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
		if (socket_fd == -1)
		{
			std::cerr << "\033[0;31mError! Socket error\033[0m" << std::endl;
			continue;
		}
		if (connect_with_timeout(socket_fd, tmp->ai_addr, tmp->ai_addrlen, 1) == -1)
		{
			std::cerr << "\033[0;31mError! Connect error\033[0m" << std::endl;
			close(socket_fd);
			continue;
		}
		break;
	}
	if (tmp == NULL)
	{
		freeaddrinfo(servinfo);
		if (socket_fd != -1)
			close(socket_fd);
		return (error_msg("\033[0;31mError! The adress is not available\033[0m", -1));
	}
	freeaddrinfo(servinfo);
	return (socket_fd);
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
