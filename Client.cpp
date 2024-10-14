/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:23:42 by tnicolau          #+#    #+#             */
/*   Updated: 2024/10/11 13:43:55 by tnicolau         ###   ########.fr       */
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

void	Client::setIPaddress(const std::string& new_ip)
{
	this->IPaddress = new_ip;
}

