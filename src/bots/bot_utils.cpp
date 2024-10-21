/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_utils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eandre <eandre@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:10:39 by eandre            #+#    #+#             */
/*   Updated: 2024/10/21 13:11:00 by eandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/bot.hpp"

int	error_msg(std::string msg, int error_nb)
{
	std::cerr << msg << std::endl;
	return (error_nb);
}
