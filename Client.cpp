/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:23:42 by tnicolau          #+#    #+#             */
/*   Updated: 2024/10/15 13:34:00 by tnicolau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"


int		Client::getFd()
{
	return this->fd;
}

void	Client::setFd(const int& new_fd)
{
	this->fd = new_fd;
}

void	Client::setPing(time_t timePing)
{
	_timePing = timePing;
}

void	Client::setPong(time_t timePong)
{
	_timePong = timePong;
}

void	Client::setIPaddress(const std::string& new_ip)
{
	this->IPaddress = new_ip;
}

