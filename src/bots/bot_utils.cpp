/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_utils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:10:39 by eandre            #+#    #+#             */
/*   Updated: 2024/10/29 09:01:18 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/bot.hpp"

int	error_msg(std::string msg, int error_nb)
{
	std::cerr << msg << std::endl;
	return (error_nb);
}

std::vector<std::string> mini_split(std::string arg, std::string to_split_on)
{
	std::vector<std::string>	targets;
	std::string					target;
	size_t						pos = arg.find_first_of(to_split_on);

	while (pos != std::string::npos)
	{
		pos = arg.find_first_of(to_split_on);
		target = arg.substr(0, pos);
		targets.push_back(target);
		arg.erase(0, pos + 1);
	}
	targets.push_back(arg);
	return (targets);
}

void	str_trim_space(std::string &str)
{
	size_t	start_pos = str.find_first_not_of(" \n\r\t\v\f\0");
	size_t	end_pos = str.find_last_not_of(" :\n\r\t\v\f\0");
	std::vector<std::string>	msg_split;

	if(start_pos != std::string::npos)
		str = str.substr(start_pos, (end_pos + 1) - start_pos);
	str = mini_split(str, " :\n\r\t\v\f\0")[0];	
}