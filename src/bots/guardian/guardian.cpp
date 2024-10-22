/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guardian.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:34:22 by eandre            #+#    #+#             */
/*   Updated: 2024/10/22 23:30:43 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/guardian.hpp"
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#define MAXDATASIZE 500

Guardian::Guardian(std::string &bot_name_, const std::string &password_, const int socket_fd_) : bot_name(bot_name_), password(password_), socket_fd(socket_fd_)
{
	pollfds[0].fd = socket_fd;
	pollfds[0].events = POLLIN;
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
	if (msg != ":42Chan 001 " + bot_name + " :Welcome to the 42Chan Network " + bot_name + "!\r\n")
		return (error_msg("\033[0;31mError! This bot is restricted to our irc only!\033[0m", 1));
	return (0);
}

int	Guardian::botjoin()
{
	size_t	pos = 0;

	pos = command.find("!botjoin ", 0);
	if (pos != 0)
		return (1);
	channel = &command[9];
	channel.erase(channel.find("\r\n", 0), 2);
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
			return (-1);
	}
	return (-1);
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
	std::vector<banned_words>::iterator it = bw.begin();
	std::string							new_word;
	size_t								pos = command.find("!addword ", 0);


	if (pos != 0)
		return (1);
	for (; it != bw.end(); it++)
	{
		if ((*it).channel == channel)
			break ;
	}
	if (it == bw.end())
		return (-1);
	if (get_word((9), new_word, command, "\r\n") == -1)
		return (-1);
	(*it).words.push_back(new_word);
	return (0);
}

int	Guardian::get_word(size_t begin_pos, std::string &word_got, std::string str, std::string last_char_to_find)
{
	size_t		pos2;
	std::string	tmp;

	if (begin_pos == std::string::npos)
		return (-1);
	tmp = str;
	pos2 = tmp.find(last_char_to_find, begin_pos);
	if (pos2 == std::string::npos)
		return (-1);
	std::cout << word_got;
	tmp.erase(pos2, tmp.length());
	std::cout << word_got;
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
	if (pos == std::string::npos)
		return (-1);
	return (get_word(pos, channel, msg, " "));
}

int	Guardian::parse_msg()
{
	std::vector<banned_words>::iterator it = bw.begin();

	// if (get_word(msg.find("#", 0), channel, msg, " ") == -1)
	// 	return (-1);
	for (; it != bw.end(); it++)
	{
		if ((*it).channel == channel)
			break ;
	}
	if (it == bw.end())
	{
		//error
		return (1);
	}
	if ((*it).words.empty())
	{
		std::cout << "hello" << std::endl; //send error to server
		return (10);
	}
	for (std::vector<std::string>::iterator it2 = (*it).words.begin(); it2 != (*it).words.end(); it2++)
	{
		if (msg.find(*it2, 0) != std::string::npos)
		{
			std::cout << *it2 << std::endl;
			return (1);
		}
	}
	return (0);
}

int	Guardian::run()
{
	int			num_bytes;
	char		buf[MAXDATASIZE];
	static int	step;

	while (42)
	{
		if (step != 0)
		{
			//===recv msg===
			
			if (poll(pollfds, 1, -1) == -1)
				return (close(socket_fd), error_msg("\033[0;31mError! Poll error\033[0m", 1));

			num_bytes = recv(socket_fd, buf, MAXDATASIZE-1, 0);
			if (num_bytes == -1)
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
				//The bot check pm in channel to moderate (inside channel) or react to a !botjoin (inside private message) accordingly
				int	status = pm_is_in_channel(msg);
				if (status == FALSE)
				{
					if (get_full_msg() == 1)
						return (close(socket_fd), 1);
					
					status = botjoin();
					if (status == -1)
						return (close(socket_fd), 1);
					else if (status == 1)
						continue ;
					step++;
				}
				else if (status == TRUE)
				{
					//do error managment
					if (get_full_msg() == -1)
						return (close(socket_fd), 1);
					if (get_channel() == -1)
						return (close(socket_fd), 1);
						
					status = addword();
					if (status == -1)
					{
						std::cout << "oui" << std::endl;
						continue ;
					}
					else if (status == 0)
						continue ;
					
					status = parse_msg();
					if (status == -1)
						continue ;
					else if (status == 1)
						msg = "PRIVMSG @%" + channel + ": BADWORD SAID!\r\n";
				}
				else if (status == -1)
					continue ;
				std::cout << "non";
				//===get sender name===

				
				//===cleanup msg===

				
				//===setup curl cmd===


				//===execute curl cmd and parse result===


				//===find and prep ai msg for send===
				
				
				//===send msg===

				if (send(socket_fd, msg.c_str(), msg.length(), 0) == -1)
					return (close(socket_fd), error_msg("\033[0;31mError! Send error\033[0m", 1));
				
				break ;
			}
			case 3:
			{
				//TODO server error parsing
				step--;
				break ;
			}
		}

		//===clear everything===

		buf[0] = '\0';
		msg.clear();
		channel.clear();
		guardian_msg.clear();
		sender_name.clear();
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