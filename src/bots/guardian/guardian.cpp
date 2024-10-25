/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guardian.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:34:22 by eandre            #+#    #+#             */
/*   Updated: 2024/10/26 00:04:45 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/guardian.hpp"
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#define MAXDATASIZE 500

bool Guardian::should_run = true;

void Guardian::handle_signal(int signum)
{
	if (signum == SIGINT)
		should_run = false;
}

Guardian::Guardian(std::string &bot_name_, const std::string &password_, const int socket_fd_) : bot_name(bot_name_), password(password_), socket_fd(socket_fd_)
{
	pollfds[0].fd = socket_fd;
	pollfds[0].events = POLLIN;
	signal(SIGINT, handle_signal);
}

Guardian::~Guardian()
{}

bool	Guardian::is_password_incorrect()
{
	return (msg == ((":42chan 464 ") + bot_name + (" :Password is incorrect or was not provided\r\n")) 
	|| msg == ":42chan 461 guest PASS :Not enough prameters\r\n:42chan 464 guest :Password is incorrect or was not provided\r\n");
}

int	Guardian::parse_connection_errors()
{
	if (is_password_incorrect())
		return (error_msg("\033[0;31mError! Incorrect password\033[0m", 1));
	if (is_name_incorrect())
		return (error_msg("\033[0;31mError! Incorrect name\033[0m", 1));
	if (msg.find(":42Chan 001 " + bot_name + " :Welcome to the 42Chan Network " + bot_name + "!\r\n") != 0)
		return (error_msg("\033[0;31mError! This bot is restricted to our irc only!\033[0m", 1));
	return (0);
}

bool	is_str_spaces(std::string str)
{
	return (str.find_first_not_of(" \n\r\t\v\f\0") == std::string::npos);
}

int	Guardian::botjoink()
{
	size_t		pos = 0;
	std::string	channel_password;

	pos = command.find("!botjoink ", 0);
	if (pos != 0)
		return (1); // CLIENT error
	if (command.find_first_of(",") != std::string::npos)
		return (CLIENT_ERROR); // CLIENT error
	if (get_word(10, channel, command, " :") == -1 || is_str_spaces(&command[11 + channel.length()]))
		return (PARAM_ERROR); // PARAM error
	if (get_word(11 + channel.length(), channel_password, command, " :\r") == -1)
		return (SERVER_ERROR); // SERVER error
	msg = "JOIN " + channel + " " + channel_password + "\r\n";
	bw.push_back((banned_words){.channel = channel, .words = std::vector<std::string>()});
	return (0);
}

int	Guardian::botjoin()
{
	size_t	pos = 0;

	pos = command.find("!botjoin ", 0);
	if (pos != 0)
		return (1); // CLIENT error
	if (command.find_first_of(",") != std::string::npos)
		return (CLIENT_ERROR); // CLIENT error
	if (get_word(9, channel, command, " :\r") == -1)
		return (-1); // SERVER error
	msg = "JOIN " + channel + "\r\n";
	bw.push_back((banned_words){.channel = channel, .words = std::vector<std::string>()});
	return (0);
}

int	pm_is_in_channel(std::string msg)
{
	size_t	pos = msg.find("PRIVMSG", 0);
	size_t	pos2 = 0;

	if (pos != std::string::npos)
	{
		pos2 = msg.find("#", pos);
		if (pos2 != std::string::npos)
		{
			pos = msg.find(":", pos);
			if (pos > pos2)
				return (TRUE);
			else
				return (FALSE);
		}
		else
			return (-1); // CLIENT error
	}
	return (-1); //not privmsg sent
}

int	launch_guardian(std::string bot_name, std::string password, int socket_fd)
{
	Guardian	guardian(bot_name, password, socket_fd);

	if (guardian.run() == 1)
		return (1);
	return (0);
}

int	Guardian::addword()
{
	std::vector<banned_words>::iterator it_bw = bw.begin();
	std::vector<std::string>::iterator	it_word;
	std::string							new_word;
	size_t								pos = command.find("!addword ", 0);


	if (pos != 0)
		return (1);
	for (; it_bw != bw.end(); it_bw++)
	{
		if ((*it_bw).channel == channel)
			break ;
	}
	if (it_bw == bw.end())
		return (-1); // SERVER error
	if (get_word((9), new_word, command, " \r") == -1)
		return (-1); // SERVER error
	for (it_word = (*it_bw).words.begin() ; it_word != (*it_bw).words.end(); it_word++)
	{
		if ((*it_word) == new_word)
			break ;
	}
	if (it_word == (*it_bw).words.end())
		(*it_bw).words.push_back(new_word);
	else
		return (CLIENT_ERROR); //CLIENT error
	return (0);
}

int	Guardian::get_word(size_t begin_pos, std::string &word_got, std::string str, std::string last_char_to_find)
{
	size_t		pos_end;
	std::string	tmp;

	if (begin_pos == std::string::npos)
		return (-1);
	tmp = str;
	pos_end = tmp.find_first_of(last_char_to_find, begin_pos);
	if (pos_end == std::string::npos)
		return (-1);
	tmp.erase(pos_end, tmp.length() - pos_end);
	word_got = &tmp[begin_pos];
	return (0);
}

int	Guardian::get_full_msg()
{
	size_t pos = msg.find(":", 1);
	if (pos == std::string::npos)
		return (-1);
	command = &msg[pos + 1];
	if (command.empty())
		return (-1);
	return (0);
}

int	Guardian::get_channel()
{
	size_t pos = msg.find("#", 0);
	return (get_word(pos, channel, msg, " "));
}

int	Guardian::get_sender_name()
{
	return (get_word(1, sender_name, msg, " "));
}

int	Guardian::rmword()
{
	std::vector<banned_words>::iterator it_bw = bw.begin();
	std::vector<std::string>::iterator	it_word;
	std::string							word_to_rm;
	size_t								pos = command.find("!rmword ", 0);


	if (pos != 0)
		return (1); //CLIENT error
	for (; it_bw != bw.end(); it_bw++)
	{
		if ((*it_bw).channel == channel)
			break ;
	}
	if (it_bw == bw.end())
		return (-1); //SERVER error
	if (get_word((8), word_to_rm, command, " \r") == -1)
		return (-1); //SERVER error
	it_word = (*it_bw).words.begin();
	for (; it_word != (*it_bw).words.end(); it_word++)
	{
		if ((*it_word) == word_to_rm)
			break ;
	}
	if (it_word == (*it_bw).words.end())
		return (CLIENT_ERROR); //CLIENT error (different)
	(*it_bw).words.erase(it_word);
	return (0);
}

int	Guardian::cleanword()
{
	std::vector<banned_words>::iterator it_bw = bw.begin();
	std::string							word_to_rm;
	size_t								pos = command.find("!cleanword\r", 0);


	if (pos != 0)
		return (1); //CLIENT error
	for (; it_bw != bw.end(); it_bw++)
	{
		if ((*it_bw).channel == channel)
			break ;
	}
	if (it_bw == bw.end())
		return (-1); //SERVER error
	if ((*it_bw).words.empty())
		return (CLIENT_ERROR); //CLIENT error (different)
	(*it_bw).words.erase((*it_bw).words.begin(), (*it_bw).words.end());
	return (0);
}

std::vector<std::string> mini_split(std::string arg, std::string to_split_on)
{
	std::vector<std::string> targets;
	size_t pos = arg.find_first_of(to_split_on);
	std::string target;

	while (pos != std::string::npos)
	{
		pos = arg.find_first_of(to_split_on);
		target = arg.substr(0, pos);
		targets.push_back(target);
		arg.erase(0, pos + 1);
	}
	targets.push_back(arg);
	return (targets);
}

int	Guardian::does_msg_contain_badword()
{
	std::vector<banned_words>::iterator it_bw = bw.begin();
	std::vector<std::string>			msg_split;

	for (; it_bw != bw.end(); it_bw++)
	{
		if ((*it_bw).channel == channel)
			break ;
	}
	if (it_bw == bw.end())
		return (-1); //SERVER error
	if ((*it_bw).words.empty())
		return (FALSE); //No error, just nothing to parse
	msg_split = mini_split(command, " ,.\r");
	for (std::vector<std::string>::iterator it_msg = msg_split.begin(); it_msg != msg_split.end(); it_msg++)
		for (std::vector<std::string>::iterator it_word = (*it_bw).words.begin(); it_word != (*it_bw).words.end(); it_word++)
			if ((*it_msg).find(*it_word, 0) != std::string::npos)
				if ((*it_msg).length() == (*it_word).length())
					return (TRUE); //BADWORD
	return (FALSE); //no BADWORD
}

int	Guardian::botleave()
{
	size_t								pos = command.find("!botleave\r", 0);
	std::vector<banned_words>::iterator it;

	if (pos != 0)
		return (1);
	if (cleanword() == -1)
		return (-1);
	for (it = bw.begin(); it != bw.end(); it++)
	{
		if ((*it).channel == channel)
			break ;
	}
	bw.erase(it);
	msg = "PART " + channel + "\r\n";
	return (0);
}

int	Guardian::run()
{
	int			num_bytes;
	char		buf[MAXDATASIZE];
	static int	step;

	while (should_run)
	{
		//===clear everything===
		
		buf[0] = '\0';
		msg.clear();
	
		if (step != 0)
		{
			//===recv msg===
			
			if (poll(pollfds, 1, -1) == -1 && should_run)
				return (close(socket_fd), error_msg("\033[0;31mError! Poll error\033[0m", 1));

			num_bytes = recv(socket_fd, buf, MAXDATASIZE-1, 0);
			if (num_bytes == -1 && should_run)
				return (close(socket_fd), error_msg("\033[0;31mError! Recv error\033[0m", 1));
			if (num_bytes == 0)
				return (close(socket_fd), error_msg("\033[0;31mError! The server closed\033[0m", 1));

			buf[num_bytes] = '\0';
			msg = buf;
			std::cout << msg;
		}
		// 3 steps for going through the server: first connection, then parsing of log in error, then msg management 
		switch (step)
		{
			case 0:
			{

				//===log into server===

				if (log_into_server() == 1)
					return (close(socket_fd), 1);
				
				step++;
				break ;
			}
			case 1:
			{

				//===parse connection errors===

				size_t	pos = bot_name.find_first_of(": ", 0);
				if (pos != std::string::npos)
					bot_name.erase(pos, bot_name.length());
				if (parse_connection_errors() == 1)
					return (close(socket_fd), 1);
				
				step++;
				break ;
			}
			case 2:
			{
				int	status = pm_is_in_channel(msg);
				//enum toggle get_msg_info(status) (FALSE = get_msg + get_sender_name, TRUE = get_msg + get_sender_name + get_channel, -1 = no get)
				if (status == FALSE)
				{
					if (get_full_msg() == -1)
						return (close(socket_fd), 1);
					if (get_sender_name() == -1)
						return (close(socket_fd), 1); //// REMOVING TOMORROW

					//status = manage_pm_request
					//if (status == server_error)
					//	leave
					//else if (status == NO_REQUEST (=1))
					//	continue ;
					//else 
					// send/continue
					status = botjoin();
					if (status == SERVER_ERROR)
						return (close(socket_fd), 1);
					else if (status == CLIENT_ERROR)
					{
						msg = "PRIVMSG " + sender_name + ": Error! This bot can't join multiple channels at once!\r\n";
						if (send(socket_fd, msg.c_str(), msg.length(), 0) == -1)
							return (close(socket_fd), error_msg("\033[0;31mError! Send error\033[0m", 1));
						continue ;
					}
					else if (status == 0)
					{
						if (send(socket_fd, msg.c_str(), msg.length(), 0) == -1)
							return (close(socket_fd), error_msg("\033[0;31mError! Send error\033[0m", 1));
						continue ;
					}
					status = botjoink();
					if (status == SERVER_ERROR)
						return (close(socket_fd), 1);
					else if (status == CLIENT_ERROR)
					{
						msg = "PRIVMSG " + sender_name + ": Error! This bot can't join multiple channels at once!\r\n";
						if (send(socket_fd, msg.c_str(), msg.length(), 0) == -1)
							return (close(socket_fd), error_msg("\033[0;31mError! Send error\033[0m", 1));
						continue ;
					}
					else if (status == 0)
					{
						if (send(socket_fd, msg.c_str(), msg.length(), 0) == -1)
							return (close(socket_fd), error_msg("\033[0;31mError! Send error\033[0m", 1));
						continue ;
					}
					else if (status == PARAM_ERROR)
					{
						msg = "PRIVMSG " + sender_name + ": Error! Not enough arguments for this command!\r\n";
						if (send(socket_fd, msg.c_str(), msg.length(), 0) == -1)
							return (close(socket_fd), error_msg("\033[0;31mError! Send error\033[0m", 1));
						continue ;
					}
					else
						continue ;
				}
				else if (status == TRUE)
				{
					if (get_full_msg() == -1)
						return (close(socket_fd), 1);
					if (get_channel() == -1)
						return (close(socket_fd), 1); //// REMOVING TOMORROW
					if (get_sender_name() == -1)
						return (close(socket_fd), 1);
					//status = manage_channel_cmd_request
					//if (status == server_error)
					//	leave
					//else if (status == NO_REQUEST)
					//	continue ;
					//else
					//	send/continue
					status = addword();
					if (status == -1)
						return (close(socket_fd), 1);
					else if (status == CLIENT_ERROR)
					{
						msg = "PRIVMSG " + channel + ": Error! You can't have multiples of the same word!\r\n";
						if (send(socket_fd, msg.c_str(), msg.length(), 0) == -1)
							return (close(socket_fd), error_msg("\033[0;31mError! Send error\033[0m", 1));
						continue ;
					}
					else if (status == 0)
						continue ;
					status = rmword();
					if (status == -1)
						return (close(socket_fd), 1);
					else if (status == CLIENT_ERROR)
					{
						msg = "PRIVMSG " + channel + ": Error! This word is not in the badword list yet!\r\n";
						if (send(socket_fd, msg.c_str(), msg.length(), 0) == -1)
							return (close(socket_fd), error_msg("\033[0;31mError! Send error\033[0m", 1));
						continue ;
					}
					else if (status == 0)
						continue ;
					status = cleanword();
					if (status == -1)
						return (close(socket_fd), 1);
					else if (status == CLIENT_ERROR)
					{
						msg = "PRIVMSG " + channel + ": Error! Nothing to clean yet!\r\n";
						if (send(socket_fd, msg.c_str(), msg.length(), 0) == -1)
							return (close(socket_fd), error_msg("\033[0;31mError! Send error\033[0m", 1));
						continue ;
					}
					else if (status == 0)
						continue ;
					status = botleave();
					if (status == -1)
						return (close(socket_fd), 1);
					else if (status == 0)
					{
						if (send(socket_fd, msg.c_str(), msg.length(), 0) == -1)
							return (close(socket_fd), error_msg("\033[0;31mError! Send error\033[0m", 1));
						continue ;
					}
					//status = manage_channel_badword
					//if (status == server_error)
					//	leave
					//else if (status == NO_REQUEST)
					//	continue ;
					//else
					//	send/continue
					status = does_msg_contain_badword();
					if (status == -1)
						return (close(socket_fd), 1);
					else if (status == TRUE)
						msg = "PRIVMSG @%" + channel + ": BADWORD SAID BY " + sender_name + "\r\n";
					else if (status == FALSE)
						continue ;
				}
				else if (status == -1)
				{
					//if (parse error() == 0)
					//	continue ;
					if (msg == ":" + bot_name + " JOIN :" + channel +"\r\n")
						continue ;
					if (msg.find(bot_name + " " + channel + " :Cannot join channel (+") == 12
						|| msg.find(bot_name + " " + channel + " Cannot join channel: ") == 12)
						msg = "PRIVMSG " + sender_name + ":Error! This bot can't join this channel!\r\n";
					else if (msg.find((":42chan 353 " + bot_name + " = " + channel + " :@" + bot_name + "\r\n")) == 0)
						msg = "PART " + channel + "\r\n" + "PRIVMSG " + sender_name + ":Error! This bot can't create new channels!\r\n";
					else
						continue ;
				}

				if (send(socket_fd, msg.c_str(), msg.length(), 0) == -1)
					return (close(socket_fd), error_msg("\033[0;31mError! Send error\033[0m", 1));
			
				break ;
			}
		}
	}

	close(socket_fd);
	return (0);
}

bool	Guardian::is_name_incorrect()
{
	return (msg == ":42chan 432 guest " + bot_name + " :Invalid nickname: Nickname contains invalid characters : #:,*?!@.\\t\\r\\n \r\n"
		|| msg == ":42chan 432 guest " + bot_name + " :Invalid nickname: First character of nickname must be a-z or A-Z\r\n"
		|| msg  == ":42chan 433 guest " + bot_name + " :Nickname is already taken\r\n"
		|| msg == ":42chan 431 guest :No nickname was given\r\n"
		|| msg == ":42chan 461 " + bot_name + " USER :Not enough prameters\r\n"
		|| msg == ":42chan 468 " + bot_name + " :Invalid username: username contains invalid characters: #:,*?!@.\\t\\r\\n \r\n"
		|| msg == ":42chan 417 guest :Message is too long\r\n"
		|| msg == ":42chan 468 " + bot_name + " :Invalid username: username is too long\r\n");
}

int	Guardian::log_into_server()
{
	msg = "PASS " + password + "\r\n";
	if (send(socket_fd, msg.c_str(), msg.length(), 0) == -1)
		return (error_msg("\033[0;31mError! Send error\033[0m", 1));
	msg.clear();
	usleep(50);
	msg = "NICK " + bot_name + "\r\n";
	if (send(socket_fd, msg.c_str(), msg.length(), 0) == -1)
		return (error_msg("\033[0;31mError! Send error\033[0m", 1));
	msg.clear();
	usleep(50);
	msg = "USER " + bot_name + " * *:" + bot_name + "\r\n";
	if (send(socket_fd, msg.c_str(), msg.length(), 0) == -1)
		return (error_msg("\033[0;31mError! Send error\033[0m", 1));
	return (0);
}

int	main(int argc, char **argv)
{
	std::string		bot_name = "GuardianBot";
	std::string 	password;
	int				socket_fd;

	//===arg management===
	
	if (argc != 4 && argc != 5)
		return (error_msg("\033[0;31mUsage : ./bot <IP addr of server> <Port of server> <Server password> <Name (optionnal)>\033[0m", 1));
	try
	{
		if (argc == 5)
			bot_name = argv[4];
		password = argv[3];

		//===connection to server===

		socket_fd = get_socket_fd(argv);
		if (socket_fd == -1)
			return (1);
		
		//===launch guardian===

		if (launch_guardian(bot_name, password, socket_fd) == 1)
			return (1);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
		return (1);
	}
}



// 	if (pm_is_in_channel(msg) == false)
// 	{
// 		int i = parse_botjoin_cmd(msg, &channel);
// 		if (i == -1)
// 			return (close(socket_fd), -1);
// 		else if (i == 1)
// 			return (close(socket_fd), 0);
// 		std::cout << channel << std::endl;
// 		if (channel.find_first_of(" :,", 0) != std::string::npos || channel.find(7, 0) != std::string::npos)
// 		{
// 			msg = "PRIVMSG TMP: Error \"";
// 			msg.append(channel);
// 			msg.append("\" can't be created because channel name contains invalid characters:\":, \"");
// 			send(socket_fd, msg.c_str(), msg.length(), 0);
// 			close(socket_fd);
// 			return (1);
// 		}
// 		msg = "JOIN ";
// 		msg.append(channel);
// 		msg.append("\r\n");
// 		send(socket_fd, msg.c_str(), msg.length(), 0);
// 		close(socket_fd);
// 		return (0);
// 	}
// 	else
// 	{
// 		close(socket_fd);
// 		return (0);
// 	}
// 	// return (1);
// 	// size_t	pos = 0;
// 	// size_t	pos2 = 0;
// 	// pos = msg.find("#", 0);
// 	// if (pos != std::string::npos)
// 	// 	pos2 = msg.find(" ", pos);
// 	// msg[pos2] = '\0';
// 	// // std::cout << pos << std::endl;
// 	// std::string msg2 = "PRIVMSG @%#";
// 	// std::string channel;
// 	// if (pos == std::string::npos)
// 	// {
// 	// 	channel = &msg[msg.length()];
// 	// 	// std::cout << " Missing # (this is not supposed to happen in normal situations)" << std::endl;
// 	// }
// 	// else
// 	// 	channel = &msg[pos + 1];
// 	// msg2.append(channel);
// 	// msg2.append(" : MY GOD NO DON'T DO THAT\r\n");
// 	// // for (std::vector<std::string>::iterator it = slurs.begin(); it != slurs.end(); it++)
// 	// // {
// 	// 	if (msg.find("tranny", 0) != std::string::npos)
// 	// 		send(socket_fd, msg2.c_str(), msg2.length(), 0);
// 	// // }
// 	close(socket_fd);
// 	return (1);
// }