/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guardian_pm_cmd.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:01:13 by eandre            #+#    #+#             */
/*   Updated: 2024/11/05 12:26:35 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/guardian.hpp"

int	Guardian::manage_pm_request()
{
	int	status;

	//===botjoin===

	status = botjoin();
	if (status == SERVER_ERROR)
		return (error_msg("\033[0;31mError! Server error on botjoin!\033[0m", -1));
	else if (status == CLIENT_ERROR)
	{
		msg = "PRIVMSG " + sender_name + ": Error! This bot can't join multiple channels at once!\r\n";
		return (0);
	}
	else if (status == 0)
		return (0);
	else if (status == PARAM_ERROR)
	{
		msg = "PRIVMSG " + sender_name + ":Usage :!botjoin <#channel>\r\n";
		return (0);
	}
	
	//===botjoink===

	status = botjoink();
	if (status == SERVER_ERROR)
		return (error_msg("\033[0;31mError! Server error on botjoink!\033[0m", -1));
	else if (status == CLIENT_ERROR)
	{
		msg = "PRIVMSG " + sender_name + ": Error! This bot can't join multiple channels at once!\r\n";
		return (0);
	}
	else if (status == 0)
		return (0);
	else if (status == PARAM_ERROR)
	{
		msg = "PRIVMSG " + sender_name + ":Usage :!botjoink <#channel> <channel_key>\r\n";
		return (0);
	}
	else
		return (NO_REQUEST);
	
	return (0);
}

int	Guardian::botjoink()
{
	size_t		pos = command.find("!botjoink", 0);
	std::string	channel_password;

	if (pos != 0)
		return (NO_REQUEST);
	if (command[9] == '\r')
		return (PARAM_ERROR);
	if (command[9] != ' ')
		return (NO_REQUEST);
	
	if (command.find_first_of(",") != std::string::npos)
		return (CLIENT_ERROR);
	if (get_word(10, channel, command, " :") == -1 || is_str_spaces(channel))
		return (PARAM_ERROR);
	if (get_word(11 + channel.length(), channel_password, command, " :\r") == -1)
		return (SERVER_ERROR);
	if (is_str_spaces(channel_password) || command[11 + channel.length()] == '\r')
		return (PARAM_ERROR);
	
	msg = "JOIN " + channel + " " + channel_password + "\r\n";
	bw.push_back((banned_words){.channel = channel, .words = std::vector<std::string>()});
	return (SUCCESS);
}

int	Guardian::botjoin()
{
	size_t	pos = command.find("!botjoin", 0);

	if (pos != 0)
		return (NO_REQUEST);
	if (command[8] == '\r')
		return (PARAM_ERROR);
	if (command[8] != ' ')
		return (NO_REQUEST);
	
	if (command.find_first_of(",") != std::string::npos)
		return (CLIENT_ERROR);
	if (get_word(9, channel, command, " :\r") == -1)
		return (SERVER_ERROR);
	if (is_str_spaces(channel) == true)
		return (PARAM_ERROR);
	
	msg = "JOIN " + channel + "\r\n";
	bw.push_back((banned_words){.channel = channel, .words = std::vector<std::string>()});
	return (SUCCESS);
}
