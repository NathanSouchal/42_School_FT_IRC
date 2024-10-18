/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numerics.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouchal <nsouchal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:09:25 by nsouchal          #+#    #+#             */
/*   Updated: 2024/10/18 14:39:18 by nsouchal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICS_HPP
# define NUMERICS_HPP

# include "Server.hpp"

#define SERVER "FT_IRC"
#define NETWORKNAME "hier c, aujourd'hui c++"
#define ERR_NEEDMOREPARAMS(source, command) (std::string(":") + SERVER + " 461 " + std::string(source) + " " + std::string(command) + " :Not enough parameters\r\n")
#define ERR_NOTREGISTERED(source, command) (std::string(":") + SERVER + " 451 " + std::string(source) + " " + std::string(command) + " :You have not registered\r\n")
#define ERR_ALREADYREGISTERED(source, command) (std::string(":") + SERVER + " 462 " + std::string(source) + " " + std::string(command) + " :You may not reregister\r\n")
#define ERR_PASSWDMISMATCH(source, command) (std::string(":") + SERVER + " 464 " + std::string(source) + " " + std::string(command) + " :Password incorrect\r\n")
#define RPL_WELCOME(nick, username, host) (std::string(":") + SERVER + " 001 " + std::string(nick) + " :Welcome to the " + NETWORKNAME + " Network, " + std::string(nick) + "!" + std::string(username) + "@" + std::string(host) + "\r\n")
#define RPL_YOURHOST(nick) (std::string(":") + SERVER + " 002 " + std::string(nick) + " :Your host is " + SERVER + ", running version 42\r\n")
#define RPL_CREATED(nick, date) (std::string(":") + SERVER + " 003 " + std::string(nick) + " :This server was created " + std::string(date) + "\r\n")
#define RPL_MYINFO(nick, usermodes, channelmodes) (std::string(":") + SERVER + " 004 " + std::string(nick) + " " + SERVER + " version 42 " + std::string(usermodes) + " " + std::string(channelmodes) + "\r\n")
#define RPL_ISUPPORT(nick, token) (std::string(":") + SERVER + " 005 " + std::string(nick) + " " + std::string(token) + " :are supported by this server" + "\r\n")
#endif
