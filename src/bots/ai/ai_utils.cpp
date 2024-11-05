/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ai_utils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:01:05 by eandre            #+#    #+#             */
/*   Updated: 2024/11/05 12:10:12 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/ai.hpp"

void Ai::handle_signal(int signum)
{
	if (signum == SIGINT)
		should_run = false;
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