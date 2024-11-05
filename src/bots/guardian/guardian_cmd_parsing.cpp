/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   guardian_cmd_parsing.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:33:31 by eandre            #+#    #+#             */
/*   Updated: 2024/11/05 12:34:06 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/guardian.hpp"

int	Guardian::parse_error()
{
	if (msg == ":" + bot_name + " JOIN :" + channel +"\r\n")
		return (0);
	if (msg.find(bot_name + " " + channel + " :Cannot join channel (+") == 12
		|| msg.find(bot_name + " " + channel + " Cannot join channel: ") == 12)
		msg = "PRIVMSG " + sender_name + ":Error! This bot can't join this channel!\r\n";
	else if (msg.find((":42chan 353 " + bot_name + " = " + channel + " :@" + bot_name + "\r\n")) == 0)
		msg = "PART " + channel + "\r\n" + "PRIVMSG " + sender_name + ":Error! This bot can't create new channels!\r\n";
	else
		return (0);
	
	return (NO_REQUEST);
}
