/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guardian.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 17:11:51 by eandre            #+#    #+#             */
/*   Updated: 2024/10/27 22:35:40 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GUARDIAN_BOT_HPP
#define GUARDIAN_BOT_HPP

#include "bot.hpp"
#include <unistd.h>
#include <poll.h>

#define TRUE 0
#define FALSE 1
#define NO_REQUEST 1
#define SUCCESS_BOTLEAVE 3
#define PARAM_ERROR 2
#define CLIENT_ERROR 10
#define SERVER_ERROR -1

struct banned_words
{
	std::string	channel;
	std::vector<std::string>	words;
};

enum is_in_channel
{
	is_in = 0,
	is_out = 1,
	is_not_privmsg = -1
};

class Guardian
{
	public:
	
		Guardian(std::string &bot_name, const std::string &password, const int socket_fd);
		~Guardian();

		int	run();
	
	private:

		std::vector<banned_words>	bw;
	
		std::string					&bot_name;
		const std::string 			&password;
	
		std::string					sender_name;
		std::string					channel;
		std::string					command;
		std::string					msg;

		const int					socket_fd;
		static bool					should_run;
		struct pollfd				pollfds[1];

		//===log in and parsing===

		int							log_into_server();
		int							pm_is_in_channel();
		int							parse_connection_errors();
		bool						is_password_incorrect();
		bool						is_name_incorrect();
		int							parse_error();
	
		//===commands management===

		int							manage_pm_request();
		int							manage_channel_cmd_request();

		//===commands===

		int							botleave();
		int							botjoin();
		int							botjoink();
		int							addword();
		int							cleanword();
		int							rmword();

		//===utils===

		int							get_sender_name();
		int							get_msg_info(int is_in_channel);
		int							get_word(size_t pos_, std::string &word, std::string str, std::string to_find);
		int							get_full_msg();
		int							get_channel();
		int							does_msg_contain_badword();
		static void 				handle_signal(int signum);

};

#endif