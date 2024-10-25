/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guardian.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 17:11:51 by eandre            #+#    #+#             */
/*   Updated: 2024/10/25 23:37:10 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GUARDIAN_BOT_HPP
#define GUARDIAN_BOT_HPP

#include "bot.hpp"
#include <vector>
#define TRUE 0
#define FALSE 1
#define NO_REQUEST 1
#define SUCCESS 0
#define PARAM_ERROR 2
#define CLIENT_ERROR 10
#define SERVER_ERROR -1

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
		static void 				handle_signal(int signum);
		static bool					should_run;
		std::vector<banned_words>	bw;
		struct pollfd				pollfds[1];
		std::string					&bot_name;
		const std::string 			&password;
		std::string					msg;
		std::string					channel;
		std::string					sender_name;
		const int					socket_fd;
		std::string					command;
		int							botleave();
		int							botjoin();
		int							botjoink();
		int							addword();
		int							cleanword();
		int							rmword();
		int							does_msg_contain_badword();
		int							log_into_server();
		int							parse_connection_errors();
		int							get_sender_name();
		int							get_word(size_t pos_, std::string &word, std::string str, std::string to_find);
		int							get_full_msg();
		int							get_channel();
		bool						is_password_incorrect();
		bool						is_name_incorrect();

};

#endif