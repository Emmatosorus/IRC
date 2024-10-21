/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guardian.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 17:11:51 by eandre            #+#    #+#             */
/*   Updated: 2024/10/21 16:46:58 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GUARDIAN_BOT_HPP
#define GUARDIAN_BOT_HPP

#include "bot.hpp"
#include <vector>
#define TRUE 0
#define FALSE 1

typedef struct banned_words
{
	std::string	channel;
	std::vector<std::string>	words;
}	banned_words;

class Guardian
{
	public:
		Guardian(std::string &bot_name, const std::string &password, const int socket_fd);
		~Guardian();
		int	run();
	private:
		std::vector<banned_words>	bw;
		struct pollfd				pollfds[1];
		std::string					&bot_name;
		const std::string 			&password;
		std::string					msg;
		std::string					channel;
		std::string					sender_name;
		std::string					guardian_msg;
		const int					socket_fd;
		int							botjoin();
		int							addword();
		int							parse_msg();
		int							log_into_server();
		int							parse_connection_errors();
		int							get_sender_name();
		int							get_word(size_t pos_, std::string &word);
		bool						is_password_incorrect();
		bool						is_name_incorrect();

};

#endif