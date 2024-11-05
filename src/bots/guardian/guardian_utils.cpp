/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guardian_utils.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:01:16 by eandre            #+#    #+#             */
/*   Updated: 2024/11/05 14:21:55 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/guardian.hpp"

void Guardian::handle_signal(int signum)
{
	if (signum == SIGINT)
		should_run = false;
}

int	Guardian::pm_is_in_channel()
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
			return (FALSE);
	}
	return (-1);
}

int	Guardian::get_msg_info(int is_in_channel)
{
	if (is_in_channel == is_in || is_in_channel == is_out)
	{
		if (get_full_msg() == -1)
			return (-1);
		if (get_sender_name() == -1)
			return (-1);
		if (is_in_channel == is_in && get_channel() == -1)
			return (-1);
	}
	return (SUCCESS);
}

int	Guardian::get_full_msg()
{
	size_t	pos = msg.find(":", 1);

	if (pos == std::string::npos)
		return (SERVER_ERROR);
	command = &msg[pos + 1];
	if (command.empty())
		return (SERVER_ERROR);
	return (SUCCESS);
}

int	Guardian::get_channel()
{
	size_t	pos = msg.find("#", 0);
	return (get_word(pos, channel, msg, " "));
}

int	Guardian::get_sender_name()
{
	return (get_word(1, sender_name, msg, " "));
}

int	Guardian::get_word(size_t begin_pos, std::string &word_got, std::string str, std::string last_char_to_find)
{
	size_t		pos_end;
	std::string	tmp;

	if (begin_pos == std::string::npos)
		return (SERVER_ERROR);
	tmp = str;
	pos_end = tmp.find_first_of(last_char_to_find, begin_pos);
	if (pos_end == std::string::npos)
		return (SERVER_ERROR);
	tmp.erase(pos_end, tmp.length() - pos_end);
	word_got = &tmp[begin_pos];
	return (SUCCESS);
}

bool	is_str_spaces(std::string str)
{
	return (str.find_first_not_of(" \n\r\t\v\f\0") == std::string::npos);
}