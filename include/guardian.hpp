/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guardian.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 17:11:51 by eandre            #+#    #+#             */
/*   Updated: 2024/11/05 15:05:51 by eandre           ###   ########.fr       */
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

#define bot_cmd_ "/*************************************************************\\\n" \
                  "| \x1b[1;4;35mBot commands :\x1b[0m                                              |\n" \
                  "|                                                             |\n" \
                  "|    \x1b[4;1;36mPRIVMSG to guardian\x1b[0m                                      |\n" \
                  "|    \x1b[1m!botjoin\x1b[0m : \x1b[32mAdds bot to channel\x1b[0m                           |\n" \
                  "|       !botjoin [channel]                                    |\n" \
                  "|    \x1b[1m!botjoink\x1b[0m : \x1b[32mAdds bot to channel with password\x1b[0m            |\n" \
                  "|       !botjoink [channel] [key]                             |\n" \
                  "|                                                             |\n" \
                  "|    \x1b[4;1;36mPRIVMSG on channel\x1b[0m                                       |\n" \
                  "|    \x1b[1m!addword\x1b[0m : \x1b[32mAdds word to censor dictionnary\x1b[0m               |\n" \
                  "|        !addword [word]                                      |\n" \
                  "|    \x1b[1m!rmword\x1b[0m : \x1b[32mremoves word from censor dictionnary\x1b[0m           |\n" \
                  "|        !rmword [word]                                       |\n" \
                  "|    \x1b[1m!cleanword\x1b[0m : \x1b[32mremoves all words from censor dictionnary\x1b[0m   |\n" \
                  "|        !cleanword                                           |\n" \
                  "|    \x1b[1m!botleave\x1b[0m : \x1b[32mremoves bot from channel\x1b[0m                     |\n" \
                  "|        !botleave                                            |\n" \
                  "\\*************************************************************/" \

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

		int							split_and_manage_msg();
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

bool	is_str_spaces(std::string str);
int		launch_guardian(std::string bot_name, std::string password, int socket_fd);

#endif