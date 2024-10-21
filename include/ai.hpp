/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:39:36 by eandre            #+#    #+#             */
/*   Updated: 2024/10/21 13:09:24 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AI_HPP
#define AI_HPP

#include "bot.hpp"

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
				\"content\": \"say hello\" \
			} \
		] \
	}'"

class Ai
{
	public:
		Ai(std::string &bot_name, const std::string &password, const int socket_fd);
		~Ai();
		int	run();
	private:
		struct pollfd		pollfds[1];
		std::string			&bot_name;
		const std::string 	&password;
		std::string			msg;
		std::string			sender_name;
		std::string			ai_msg;
		std::string			curl_cmd;
		const int			socket_fd;
		int					log_into_server();
		int					parse_connection_errors();
		int					get_sender_name();
		int					cleanup_msg();
		int					setup_curl_cmd();
		int					execute_curl_cmd_and_parse_result();
		int					find_and_prep_ai_msg_for_send();
		bool				is_password_incorrect();
		bool				is_name_incorrect();
};

#endif