/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai_run.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 18:19:04 by eandre            #+#    #+#             */
/*   Updated: 2024/11/05 15:59:37 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/ai.hpp"

bool Ai::should_run = true;

int	Ai::run()
{
	int	status;

	while (should_run)
	{
		//===split and manage msg===

		status = split_and_manage_msg();
		if (status == CONTINUE)
			continue ;
		if (status == 1)
			return (1);
	
	}

	close(socket_fd);
	return (SUCCESS);
}

/*
This is where everything gets interesting.
There is 3 steps for going through the server: first connection, then parsing of log in error, then msg management.
Here you will find the root of all the calls of any command.
*/

int	Ai::split_and_manage_msg()
{
	std::vector<std::string>	msg_splited;
	int							num_bytes;
	char						buf[MAXDATASIZE];
	static int					step;
	
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

	//===split message to be sure to manage one message at a time===

	msg_splited = mini_split(msg, "\r\n");
	for (std::vector<std::string>::iterator it_msg = msg_splited.begin();  it_msg != msg_splited.end(); it_msg++)
	{
		*it_msg += "\r\n";
		msg = *it_msg;

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
					return (CONTINUE);
				
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
	return (0);
}