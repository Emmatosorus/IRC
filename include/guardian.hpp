/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guardian.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 17:11:51 by eandre            #+#    #+#             */
/*   Updated: 2024/10/21 13:30:40 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GUARDIAN_BOT_HPP
#define GUARDIAN_BOT_HPP

#include "bot.hpp"
#include <vector>

class Guardian
{
	public:
		Guardian(std::string &bot_name, const std::string &password, const int socket_fd);
		~Guardian();
		int	run();
	private:
		struct pollfd		pollfds[1];
		std::string			&bot_name;
		const std::string 	&password;
		std::string			msg;
		std::string			sender_name;
		std::string			guardian_msg;
		const int			socket_fd;
		int					log_into_server();
		int					parse_connection_errors();
		int					get_sender_name();
		bool				is_password_incorrect();
		bool				is_name_incorrect();
};

#endif