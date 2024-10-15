/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:23:09 by tnicolau          #+#    #+#             */
/*   Updated: 2024/10/15 12:03:34 by tnicolau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <ctime>

class	Client
{
	private:
		int			fd;
		std::string	IPaddress;
		time_t		_timePing;
	public:
		int		getFd();
		void	setFd(const int& new_fd);
		void	setPing(time_t timePing);
		void	setIPaddress(const std::string& new_ip);
};

#endif
