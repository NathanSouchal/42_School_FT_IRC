/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 09:23:55 by tnicolau          #+#    #+#             */
/*   Updated: 2024/10/22 09:28:12 by tnicolau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string	getTimestamp()
{
	time_t	timestamp;
	time(&timestamp);
	std::string timeString = ctime(&timestamp);
	return timeString;
}

