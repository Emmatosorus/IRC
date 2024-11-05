/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai_login.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:01:02 by eandre            #+#    #+#             */
/*   Updated: 2024/11/05 15:45:37 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/ai.hpp"

/*
To log into an irc server, a lot of parsing needs to be done.
There is a lot of server answers to break down.
To avoid the bot being stuck in the first step forever, we need to take account of theses answers.
Like always, my work is divided into step, to make it easier to read (you will thank me later)
*/

//First, login into the server. This part is pretty easy, because you just need to send the informations given in the irc norm.

int	Ai::log_into_server()
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
	return (SUCCESS);
}

//And second is trying to find EVERY errors possible, but with added security, with the last if being a full stop to everything outside of the greating of the server

int	Ai::parse_connection_errors()
{
	if (is_password_incorrect())
		return (error_msg("\033[0;31mError! Incorrect password\033[0m", 1));
	if (is_name_incorrect())
		return (error_msg("\033[0;31mError! Incorrect name\033[0m", 1));
	if (msg.find(":42Chan 001 " + bot_name + " :Welcome to the 42Chan Network " + bot_name + "!\r\n") != 0)
		return (error_msg("\033[0;31mError! This bot is restricted to our irc only!\033[0m", 1));
	
	return (SUCCESS);
}

bool	Ai::is_password_incorrect()
{
	return (msg == ((":42chan 464 ") + bot_name + (" :Password is incorrect or was not provided\r\n")) 
	|| msg == ":42chan 461 guest PASS :Not enough parameters\r\n:42chan 464 guest :Password is incorrect or was not provided\r\n"
	|| msg == ":42chan 417 guest :Message is too long\r\n:42chan 417 guest :Message is too long\r\n");
}

bool	Ai::is_name_incorrect()
{
	return (msg == ":42chan 432 guest " + bot_name + " :Invalid nickname: Nickname contains invalid characters: #%:,*?!@. \\t\\r\\n\r\n"
		|| msg == ":42chan 432 guest " + bot_name + " :Invalid nickname: First character of nickname must be a-z or A-Z\r\n"
		|| msg  == ":42chan 433 guest " + bot_name + " :Nickname is already taken\r\n"
		|| msg == ":42chan 431 guest :No nickname was given\r\n"
		|| msg == ":42chan 461 " + bot_name + " USER :Not enough parameters\r\n"
		|| msg == ":42chan 468 " + bot_name + " :Invalid username: username contains invalid characters: #%:,*?!@. \\t\\r\\n\r\n"
		|| msg == ":42chan 417 guest :Message is too long\r\n"
		|| msg == ":42chan 417 " + bot_name + " :Message is too long\r\n"
		|| msg == ":42chan 468 " + bot_name + " :Invalid username: username is too long\r\n");
}