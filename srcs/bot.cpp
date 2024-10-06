/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:34:22 by eandre            #+#    #+#             */
/*   Updated: 2024/10/06 16:29:40 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/bot.hpp"
#define MAXDATASIZE 100

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int	main(int argc, char **argv)
{
	int	socket_fd;
	struct addrinfo hints;
	struct addrinfo *tmp;
	struct addrinfo *servinfo;

	hints.ai_addr = NULL;
	hints.ai_addrlen = 0;
	hints.ai_canonname = NULL;
	hints.ai_flags = 0;
	hints.ai_addr = NULL;
	hints.ai_protocol = 0;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if (argc != 2)
	{
		std::cout << "NIQUE TA MERE" << std::endl;
		return (1);
	}
	if (getaddrinfo(argv[1], MYPORT, &hints, &servinfo) != 0)
		return (2);
	for (tmp = servinfo; tmp != NULL; tmp = tmp->ai_next)
	{
		socket_fd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
		if (socket_fd == -1)
			continue;
		if (connect(socket_fd, tmp->ai_addr, tmp->ai_addrlen) == -1)
		{
			close(socket_fd);
			continue;
		}
		break ;
	}
	if (tmp == NULL)
		return (2);
	char s[INET6_ADDRSTRLEN];
	inet_ntop(tmp->ai_family, get_in_addr((struct sockaddr *)tmp->ai_addr),s, sizeof(s));
	freeaddrinfo(servinfo);
	int	num_bytes;
	char	buf[MAXDATASIZE];

	if ((num_bytes = recv(socket_fd, buf, MAXDATASIZE-1, 0)) == -1)
		return (1);
	buf[num_bytes] = '\0';
	std::cout << "bot received: " << buf << std::endl;
	std::string	test;
	test = buf;
	size_t	pos;
	pos = test.find("Hello", 0);
	if (pos != std::string::npos)
		std::cout << "WTF" << std::endl;
	close(socket_fd);
	return (0);
}
