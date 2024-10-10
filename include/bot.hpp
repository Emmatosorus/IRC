/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 15:34:01 by eandre            #+#    #+#             */
/*   Updated: 2024/10/10 21:26:37 by eandre           ###   ########.fr       */
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

#define MYPORT "3490"
#define CURL_CMD "curl \"https://api.openai.com/v1/chat/completions\" \
			-H \"Content-Type: application/json\" \
			-H \"Authorization: Bearer $OPENAI_API_KEY\" \
			-d '{ \
			\"model\": \"gpt-4o-mini\", \
			\"messages\": [ \
			{ \
				\"role\": \"system\", \
				\"content\": \"You are a helpful assistant.\" \
			}, \
			{ \
				\"role\": \"user\", \
				\"content\": \"say ' hello\" \
			} \
		] \
	}' > Chatgpt_reply.txt"

#endif