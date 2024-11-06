/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai_innit.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:13:28 by eandre            #+#    #+#             */
/*   Updated: 2024/11/05 12:13:49 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/ai.hpp"

Ai::Ai(std::string& bot_name_, const std::string& password_, const int socket_fd_)
    : bot_name(bot_name_), password(password_), socket_fd(socket_fd_)
{
    pollfds[0].fd = socket_fd;
    pollfds[0].events = POLLIN;
    signal(SIGINT, handle_signal);
}

Ai::~Ai() {}