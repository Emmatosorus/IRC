/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai_main.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:11:47 by eandre            #+#    #+#             */
/*   Updated: 2024/11/05 12:12:36 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/ai.hpp"

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

int	launch_ai(std::string bot_name, std::string password, int socket_fd)
{
	Ai	ai(bot_name, password, socket_fd);

	if (ai.run() == 1)
		return (1);
	return (0);
}