/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:34:22 by eandre            #+#    #+#             */
/*   Updated: 2024/10/04 15:11:47 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>

#define MYPORT "4950"

int	main(int argc, char **argv)
{
	int	socket_fd;
	struct addrinfo hints;
	struct addrinfo *servinfo;

	hints.ai_addr = NULL;
	hints.ai_addrlen = 0;
	hints.ai_canonname = NULL;
	hints.ai_family = AF_INET6; // set to AF_INET to use IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = 0;
	hints.ai_addr = NULL;
	hints.ai_protocol = 0;
	if (argc != 2)
	{
		std::cout << "NIQUE TA MERE" << std::endl;
		return (1);
	}
	if (getaddrinfo(argv[1], MYPORT, &hints, &servinfo) != 0)
		return (1);
	
	
}