/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guardian_channel_cmd.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:01:08 by eandre            #+#    #+#             */
/*   Updated: 2024/11/05 13:40:37 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/guardian.hpp"

int	Guardian::manage_channel_cmd_request()
{
	int	status;

	//===addword===

	status = addword();
	if (status == -1)
		return (error_msg("\033[0;31mError! Server error on addword!\033[0m", -1));
	else if (status == PARAM_ERROR)
	{
		msg = "PRIVMSG " + channel + ":Usage :!addword <word_to_add>\r\n";
		return (0);
	}
	else if (status == CLIENT_ERROR)
	{
		msg = "PRIVMSG " + channel + ": Error! You can't have multiples of the same word!\r\n";
		return (CLIENT_ERROR);
	}
	else if (status == 0)
		return (SUCCESS);

	//===rmword===

	status = rmword();
	if (status == -1)
		return (error_msg("\033[0;31mError! Server error on rmword!\033[0m", -1));
	else if (status == PARAM_ERROR)
	{
		msg = "PRIVMSG " + channel + ":Usage :!rmword <word_to_rm>\r\n";
		return (0);
	}
	else if (status == CLIENT_ERROR)
	{
		msg = "PRIVMSG " + channel + ": Error! This word is not in the badword list yet!\r\n";
		return (CLIENT_ERROR);
	}
	else if (status == 0)
		return (SUCCESS);
	
	//===cleanword===

	status = cleanword();
	if (status == -1)
		return (error_msg("\033[0;31mError! Server error on cleanword!\033[0m", -1));
	else if (status == CLIENT_ERROR)
	{
		msg = "PRIVMSG " + channel + ": Error! Nothing to clean yet!\r\n";
		return (CLIENT_ERROR);
	}
	else if (status == 0)
		return (SUCCESS);

	//===botleave===

	status = botleave();
	if (status == -1)
		return (error_msg("\033[0;31mError! Server error on botleave!\033[0m", -1));
	else if (status == 0)
		return (SUCCESS_BOTLEAVE);
	return (NO_REQUEST);
}

int	Guardian::addword()
{
	std::vector<banned_words>::iterator	it_bw = bw.begin();
	std::vector<std::string>::iterator	it_word;
	std::string							new_word;
	size_t								pos = command.find("!addword", 0);


	if (pos != 0)
		return (NO_REQUEST);
	if (command[8] == '\r')
		return (PARAM_ERROR);
	if (command[8] != ' ')
		return (NO_REQUEST);
	for (; it_bw != bw.end(); it_bw++)
	{
		if ((*it_bw).channel == channel)
			break ;
	}
	if (it_bw == bw.end())
		return (SERVER_ERROR);
	if (get_word((9), new_word, command, " \r") == SERVER_ERROR)
		return (SERVER_ERROR);
	if (is_str_spaces(new_word) == true)
		return (PARAM_ERROR);

	for (it_word = (*it_bw).words.begin() ; it_word != (*it_bw).words.end(); it_word++)
	{
		if ((*it_word) == new_word)
			break ;
	}
	if (it_word == (*it_bw).words.end())
		(*it_bw).words.push_back(new_word);
	else
		return (CLIENT_ERROR);

	return (SUCCESS);
}

int	Guardian::rmword()
{
	std::vector<banned_words>::iterator it_bw = bw.begin();
	std::vector<std::string>::iterator	it_word;
	std::string							word_to_rm;
	size_t								pos = command.find("!rmword", 0);


	if (pos != 0)
		return (NO_REQUEST);
	if (command[7] == '\r')
		return (PARAM_ERROR);
	if (command[7] != ' ')
		return (NO_REQUEST);
	
	for (; it_bw != bw.end(); it_bw++)
	{
		if ((*it_bw).channel == channel)
			break ;
	}
	if (it_bw == bw.end())
		return (SERVER_ERROR);
	if (get_word((8), word_to_rm, command, " \r") == -1)
		return (SERVER_ERROR);
	if (is_str_spaces(word_to_rm) == true)
		return (PARAM_ERROR);
	
	it_word = (*it_bw).words.begin();
	for (; it_word != (*it_bw).words.end(); it_word++)
	{
		if ((*it_word) == word_to_rm)
			break ;
	}
	if (it_word == (*it_bw).words.end())
		return (CLIENT_ERROR);
	(*it_bw).words.erase(it_word);

	return (SUCCESS);
}

int	Guardian::cleanword()
{
	std::vector<banned_words>::iterator it_bw = bw.begin();
	std::string							word_to_rm;
	size_t								pos = command.find("!cleanword\r", 0);


	if (pos != 0)
		return (NO_REQUEST);
	
	for (; it_bw != bw.end(); it_bw++)
	{
		if ((*it_bw).channel == channel)
			break ;
	}
	if (it_bw == bw.end())
		return (SERVER_ERROR);
	if ((*it_bw).words.empty())
		return (CLIENT_ERROR);
	(*it_bw).words.erase((*it_bw).words.begin(), (*it_bw).words.end());

	return (SUCCESS);
}

int	Guardian::botleave()
{
	std::vector<banned_words>::iterator it;
	size_t								pos = command.find("!botleave\r", 0);

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

int	Guardian::does_msg_contain_badword()
{
	std::vector<banned_words>::iterator it_bw = bw.begin();
	std::vector<std::string>			msg_split;

	std::cout << channel << std::endl;
	for (; it_bw != bw.end(); it_bw++)
	{
		if ((*it_bw).channel == channel)
			break ;
	}
	if (it_bw == bw.end())
		return (SERVER_ERROR);
	if ((*it_bw).words.empty())
		return (FALSE);
	
	msg_split = mini_split(command, " :;?!,.\r");
	for (std::vector<std::string>::iterator it_msg = msg_split.begin(); it_msg != msg_split.end(); it_msg++)
		for (std::vector<std::string>::iterator it_word = (*it_bw).words.begin(); it_word != (*it_bw).words.end(); it_word++)
			if ((*it_msg).find(*it_word, 0) != std::string::npos)
				if ((*it_msg).length() == (*it_word).length())
					return (msg = "PRIVMSG @" + channel + ": BADWORD SAID BY " + sender_name + "\r\n", TRUE);
	
	return (FALSE);
}
