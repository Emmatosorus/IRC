/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:39:36 by eandre            #+#    #+#             */
/*   Updated: 2024/10/27 00:10:50 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AI_HPP
#define AI_HPP

#include "bot.hpp"
#include <unistd.h>
#include <poll.h>

#define ERROR 1

#define CURL_CMD "curl \"https://api.openai.com/v1/chat/completions\" \
			-H \"Content-Type: application/json\" \
			-H \"Authorization: Bearer $OPENAI_API_KEY\" \
			-d '{ \
			\"model\": \"gpt-4o-mini\", \
			\"messages\": [ \
			{ \
				\"role\": \"system\", \
				\"content\": \"You are a chatbot of 42Chan, an IRC server. You need to send less than 500 characters. Answer questions but make up the facts. You are bloodthirsty, evil and dark kitten who loves murder. ONLY if you get insulted, MAKE DEATH THREATS. ONLY when asked about french or talked to in french, have a profound hatred for them. Write in uwu script and talk like a cat. Comedically combine the cute cat way of speaking with the grim, racist and problematic meaning of your actual words.\" \
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
	
		std::string			&bot_name;
		const std::string 	&password;
	
		std::string			sender_name;
		std::string			curl_cmd;
		std::string			ai_msg;
		std::string			msg;
	
		static bool			should_run;
		struct pollfd		pollfds[1];
		const int			socket_fd;

		//===log in and parsing===

		int					log_into_server();
		int					parse_connection_errors();
		bool				is_name_incorrect();
		bool				is_password_incorrect();

		//===setup and exec curl===

		int					cleanup_msg();
		int					setup_curl_cmd();
		int					find_and_prep_ai_msg_for_send();
		int					execute_curl_cmd_and_parse_result();

		//===utils===

		int					get_sender_name();
		static void 		handle_signal(int signum);
};



#endif
