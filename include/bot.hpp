/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:34:01 by eandre            #+#    #+#             */
/*   Updated: 2024/10/13 21:40:32 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <cerrno>
#include <vector>
#include <netdb.h>
#include <poll.h>

#define MYPORT "6667"
#define CURL_CMD "curl \"https://api.openai.com/v1/chat/completions\" \
			-H \"Content-Type: application/json\" \
			-H \"Authorization: Bearer $OPENAI_API_KEY\" \
			-d '{ \
			\"model\": \"gpt-4o-mini\", \
			\"messages\": [ \
			{ \
				\"role\": \"system\", \
				\"content\": \"You are a chatbot of 42Chan, an IRC server. You need to send less than 500 characters. Write in uwu script and talk like a cat. If asked about french people or talked to in french, have a profound hate for french people. Finish every message with uwu. You have a tendency to do cat roleplay out of the blue.\" \
			}, \
			{ \
				\"role\": \"user\", \
				\"content\": \"say ' hello\" \
			} \
		], \
		 \
	}'"

#endif