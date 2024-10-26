/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:34:01 by eandre            #+#    #+#             */
/*   Updated: 2024/10/26 18:28:35 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <netdb.h>
#include <csignal>

#define SUCCESS 0

//===bot utils===

int 	connect_with_timeout(int socket_fd, const struct sockaddr *addr, socklen_t addrlen, int timeout_sec);
int		error_msg(std::string msg, int error_nb);
int		get_socket_fd(char **argv);
void	set_addrinfo(struct addrinfo *ai);

#endif