/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guardian_channel_cmd.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:01:08 by eandre            #+#    #+#             */
/*   Updated: 2024/11/05 15:45:08 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/guardian.hpp"

/*
This bot has a lot of little commands, but on this file, we will focus on the channel commands only.

The channel commands are only available in channels, which means the bot won't answer in private message to any of theses :
 addword,
 rmword,
 cleanword,
 botleave.

That also means that this bot doesnt need to have the channel name as an argument,
since he will automaticly find from where the message was sent.
*/

//This function is the channel command hub, where every channel command is executed and also where every error message are sent.

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

/*
Addword is core of the commands of this bot.

The function is simple:
You add a word that is banned, which is stored in a vector of struct,
containing the channel name and the banned words from this channel, stored in a vector of string.

If somebody tries to say a banned word, the bot will send a warning to operators of this channel with the name of the sender.
Operators will kill you if they see it, so please don't do it.

To call this function, you just need to put the word you want added from the list as the argument.
*/

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

/*
Remove word is the exact oposite of addword.
If you can add something, you should be able to remove it !

Now, if somebody tries to say a banned word, the bot will send nothing.
Amazing, isn't it?

To call this function, you just need to put the word you want removed from the list as the argument.
*/

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

/*
Cleanword is basicly rmword but on steroids.

If you want to remove everything from the banned word list of a channel, use this instead of rmword.
This function takes no arguments.
*/

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

/*
And finaly, botleave.
If you want the bot to part a channel and leave nothing behind, use this command, if not, simply kick him ! (ouch)
This function takes no arguments.
*/

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

/*
If addword is the core of the commands, this is the core of the core of the commands.
This isnt a function, its utility is simply to send the message to operators in case of someone sending a bad word.
*/

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
