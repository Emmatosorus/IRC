/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:39:36 by eandre            #+#    #+#             */
/*   Updated: 2024/10/19 20:04:42 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AI_HPP
#define AI_HPP

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
};

#endif