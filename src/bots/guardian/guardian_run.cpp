/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guardian_run.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:34:22 by eandre            #+#    #+#             */
/*   Updated: 2024/11/05 15:07:04 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/guardian.hpp"

bool Guardian::should_run = true;

int	Guardian::run()
{
	int status;

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

int	Guardian::split_and_manage_msg()
{
	std::vector<std::string>	msg_splited;
	int							num_bytes;
	char						buf[MAXDATASIZE];
	static int					step;


	//===clear everything not needed===

	buf[0] = '\0';
	msg.clear();

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

				if (log_into_server() == 1)
					return (close(socket_fd), 1);
				
				step++;
				break ;
			}
			case 1:
			{

				//===parse connection errors===

				bot_name_trim(bot_name);
				if (parse_connection_errors() == 1)
					return (close(socket_fd), 1);
				std::cout << bot_cmd_ << std::endl;

				step++;
				break ;
			}
			case 2:
			{
				int	status = pm_is_in_channel();
				
				//===get message info===

				if (get_msg_info(status) == SERVER_ERROR)
					return (close(socket_fd), 1);

				if (status == FALSE)
				{
					//===manage private message request===
				
					status = manage_pm_request();
					if (status == SERVER_ERROR)
						return (close(socket_fd), 1);
					else if (status == NO_REQUEST)
						return (CONTINUE);

				}
				else if (status == TRUE)
				{

					//===manage channel command request===

					status = manage_channel_cmd_request();
					if (status == SERVER_ERROR)
						return (close(socket_fd), 1);
					else if (status == NO_REQUEST)
					{

						//===manage channel badwords===

						status = does_msg_contain_badword();
					
						if (status == SERVER_ERROR)
							return (close(socket_fd), error_msg("crash here", 1));
						else if (status == FALSE)
							return (CONTINUE);

					}
				}

				//===parse join errors===

				else if (status == -1)
					if (parse_error() == 0)
						return (CONTINUE);
				
				//===send message===

				if (send(socket_fd, msg.c_str(), msg.length(), 0) == -1)
					return (close(socket_fd), error_msg("\033[0;31mError! Send error\033[0m", 1));
				usleep(50);
			
				break ;
			}
		}
	}
	return (0);
}
