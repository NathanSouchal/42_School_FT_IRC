/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouchal <nsouchal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 13:23:42 by tnicolau          #+#    #+#             */
/*   Updated: 2024/10/17 16:03:43 by nsouchal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
	_nickname = "*";
}

const int		&Client::getFd()
{
	return _fd;
}

const std::string	&Client::getNickname()
{
	return _nickname;
}

const std::string	&Client::getUsername()
{
	return _username;
}


const std::string	&Client::getRealname()
{
	return _realname;
}

const std::string	&Client::getPassword()
{
	return _password;
}

const bool	&Client::getRegistration()
{
	return _registrationChecked;
}


void	Client::setFd(const int& new_fd)
{
	_fd = new_fd;
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
	_IPaddress = new_ip;
}

void	Client::setNick(const std::string &nickname)
{
	_nickname = nickname;
}

void	Client::setPassword(const std::string &password)
{
	_password = password;
}

void	Client::setUsername(const std::string &username)
{
	_username = username;
}

void	Client::setRealname(const std::string &realname)
{
	_realname = realname;
}

void	Client::setTrueRegistration()
{
	_registrationChecked = true;
}

bool	Client::checkRegistration()
{
	if (_username.empty() || _nickname.empty() || _password.empty() || _realname.empty())
		return false;
	return true;
}

void	Client::reply(std::string message)
{
	std::string	final_msg = ":ft_irc " + message;
	
	send(_fd, final_msg.c_str(), final_msg.size(), 0);
}