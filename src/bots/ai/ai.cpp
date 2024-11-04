/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 18:19:04 by eandre            #+#    #+#             */
/*   Updated: 2024/11/04 12:10:52 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/ai.hpp"

bool Ai::should_run = true;

void Ai::handle_signal(int signum)
{
	if (signum == SIGINT)
		should_run = false;
}

Ai::Ai(std::string &bot_name_, const std::string &password_, const int socket_fd_) : bot_name(bot_name_), password(password_), socket_fd(socket_fd_)
{
	pollfds[0].fd = socket_fd;
	pollfds[0].events = POLLIN;
	signal(SIGINT, handle_signal);
}

Ai::~Ai()
{}

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

int	Ai::get_sender_name()
{
	std::string tmp;
	size_t		pos = msg.find(" PRIVMSG", 1);

	if (pos == std::string::npos)
		return (ERROR);
	tmp = msg;
	tmp[pos] = '\0';
	tmp = &tmp[1];
	sender_name = tmp;
	return (SUCCESS);
}

bool	Ai::is_password_incorrect()
{
	return (msg == ((":42chan 464 ") + bot_name + (" :Password is incorrect or was not provided\r\n")) 
	|| msg == ":42chan 461 guest PASS :Not enough parameters\r\n:42chan 464 guest :Password is incorrect or was not provided\r\n"
	|| msg == ":42chan 417 guest :Message is too long\r\n:42chan 417 guest :Message is too long\r\n");
}

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


int	Ai::cleanup_msg()
{
	size_t	pos = msg.find_first_of("\'\"", 0);

	while (pos != std::string::npos)
	{
		msg.erase(pos, 1);
		msg.insert(pos, " ");
		pos = msg.find_first_of("\'\"", pos + 1);
	}
	pos = msg.find("\r\n", 0);
	if (pos == std::string::npos)
		return (error_msg("\033[0;31mError! Client msg has an error!\033[0m", 1));
	msg.erase(pos, 2);
	return (SUCCESS);
}

int	Ai::setup_curl_cmd()
{
	size_t	pos;
	size_t	end_pos;

	pos = curl_cmd.find("content", 0);
	if (pos == std::string::npos)
		return (error_msg("\033[0;31mError! Curl cmd has an error!\033[0m", 1));
	
	pos = curl_cmd.find("content", pos + 7);
	if (pos == std::string::npos)
		return (error_msg("\033[0;31mError! Curl cmd has an error!\033[0m", 1));

	pos += 11;
	end_pos = curl_cmd.find("\"", pos);
	if (end_pos == std::string::npos)
		return (error_msg("\033[0;31mError! Curl cmd has an error!\033[0m", 1));

	curl_cmd.erase(pos, end_pos - pos);
	curl_cmd.insert(pos, msg);
	return (SUCCESS);
}

int	Ai::execute_curl_cmd_and_parse_result()
{
	FILE	*fp = popen(curl_cmd.c_str(), "r");
	char	fgets_buf[1024];
	int 	status;
	size_t	pos = 0;

	if (fp == NULL)
		return (error_msg("\033[0;31mError! Popen error!\033[0m", 1));

	while (fgets(fgets_buf, 1024, fp) != NULL)
	{
		ai_msg = fgets_buf;
		pos = ai_msg.find("content", 0);
		if (ai_msg.find("You didn't provide an API key.", 0) != std::string::npos || ai_msg.find("Incorrect API key provided", 0) != std::string::npos)
			return (pclose(fp), error_msg("\033[0;31mError! API key error!\033[0m", 1));
		if (ai_msg.find("We could not parse the JSON body of your request.", 0) != std::string::npos)
			return (pclose(fp), error_msg("\033[0;31mError! Curl cmd has an error!\033[0m", 1));
		if (pos != std::string::npos)
			break ;
	}
	if (pos == 0)
		return (pclose(fp), error_msg("\033[0;31mError! You did a redirection again you silly goose!\033[0m", 1));
	
	status = pclose(fp);
	if (status == -1)
		return (error_msg("\033[0;31mError! Pclose error!\033[0m", 1));
	
	return (SUCCESS);
}

int	Ai::find_and_prep_ai_msg_for_send()
{
	size_t	pos = ai_msg.find("content", 0);
	size_t	end_pos = 0;
	size_t	tmp_pos = 0;

	if (pos == std::string::npos)
		return (error_msg("\033[0;31mError! Curl cmd has an error!\033[0m", 1));
	pos += 11;
	end_pos = ai_msg.find("\"", pos);
	tmp_pos = end_pos;
	while (tmp_pos != std::string::npos)
	{
		end_pos = ai_msg.find("\"", tmp_pos + 1);
		tmp_pos = ai_msg.find("\"", end_pos + 1);
	}
	if (end_pos == std::string::npos)
		return (error_msg("\033[0;31mError! Curl cmd has an error!\033[0m", 1));
	ai_msg[end_pos] = '\0';
	ai_msg = &ai_msg[pos];
	msg = "PRIVMSG " + sender_name + ": " + ai_msg + "\r\n";
	return (SUCCESS);
}

int	launch_ai(std::string bot_name, std::string password, int socket_fd)
{
	Ai	ai(bot_name, password, socket_fd);

	if (ai.run() == 1)
		return (1);
	return (0);
}

int	Ai::run()
{
	int			num_bytes;
	char		buf[MAXDATASIZE];
	static int	step;

	while (should_run)
	{
		//===clear everything===

		buf[0] = '\0';
		msg.clear();
		ai_msg.clear();
		sender_name.clear();
		curl_cmd.clear();

		if (step != 0)
		{
			//===recv msg===
			
			if (poll(pollfds, 1, -1) == -1 && should_run)
				return (close(socket_fd), error_msg("\033[0;31mError! Poll error\033[0m", 1));

			num_bytes = recv(socket_fd, buf, MAXDATASIZE - 1, 0);
			if (num_bytes == -1 && should_run)
				return (close(socket_fd), error_msg("\033[0;31mError! Recv error\033[0m", 1));
			if (num_bytes == 0)
				return (close(socket_fd), error_msg("\033[0;31mError! The server closed\033[0m", 1));

			buf[num_bytes] = '\0';
			msg = buf;
		}
		// 3 steps for going through the server: first connection, then parsing of log in error, then msg management 
		switch (step)
		{
			case 0:
			{

				//===log into server===

				if (log_into_server() == ERROR)
					return (close(socket_fd), ERROR);
				
				step++;
				break ;
			}
			case 1:
			{

				//===parse connection errors===

				bot_name_trim(bot_name);
				if (parse_connection_errors() == ERROR)
					return (close(socket_fd), ERROR);
				
				step++;
				break ;
			}
			case 2:
			{
				curl_cmd = CURL_CMD;
			
				//===get sender name===

				if (get_sender_name() == 1)
					continue ;
				
				//===cleanup msg===

				if (cleanup_msg() == ERROR)
					return (close(socket_fd), ERROR);
				
				//===setup curl cmd===

				if (setup_curl_cmd() == ERROR)
					return (close(socket_fd), ERROR);

				//===execute curl cmd and parse result===

				if (execute_curl_cmd_and_parse_result() == ERROR)
					return (close(socket_fd), ERROR);

				//===find and prep ai msg for send===
				
				if (find_and_prep_ai_msg_for_send() == ERROR)
					return (close(socket_fd), ERROR);
				
				//===send msg===

				if (send(socket_fd, msg.c_str(), msg.length(), 0) == -1)
					return (close(socket_fd), error_msg("\033[0;31mError! Send error\033[0m", 1));
				
				break ;
			}
		}
	}

	close(socket_fd);
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	std::string		bot_name = "NyanBot";
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
		
		//===launch ai===

		if (launch_ai(bot_name, password, socket_fd) == 1)
			return (1);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
		if (socket_fd != -1)
			close(socket_fd);
		return (1);
	}
}
