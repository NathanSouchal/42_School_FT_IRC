/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numerics.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:09:25 by nsouchal          #+#    #+#             */
/*   Updated: 2024/10/22 17:11:36 by tnicolau         ###   ########.fr       */
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

#define RPL_LUSERCLIENT(nick, users, invisible) (std::string(":") + SERVER + " 251 " + std::string(nick) + " :There are " + std::string(users) + " users and " + std::string(invisible) + " invisible on 1 servers\r\n")
#define RPL_LUSEROP(nick, ops) (std::string(":") + SERVER + " 252 " + std::string(nick) + " " + std::string(ops) + " :operator(s) online\r\n")
#define RPL_LUSERUNKNOWN(nick, connections) (std::string(":") + SERVER + " 253 " + std::string(nick) + " " + std::string(connections) + " :unknown connection(s)\r\n")
#define RPL_LUSERCHANNELS(nick, channels) (std::string(":") + SERVER + " 254 " + std::string(nick) + " " + std::string(channels) + " :channels formed\r\n")
#define RPL_LUSERME(nick, clients) (std::string(":") + SERVER + " 255 " + std::string(nick) + " :I have " + std::string(clients) + " clients and 1 servers\r\n")
#define RPL_LOCALUSERS(nick, nbClients, nbMaxClients) (std::string(":") + SERVER + " 265 " + std::string(nick) + " :Current local users " + std::string(nbClients) + ", max " + std::string(nbMaxClients) + "\r\n")
#define RPL_GLOBALUSERS(nick, nbClients, nbMaxClients) (std::string(":") + SERVER + " 266 " + std::string(nick) + " :Current global users " + std::string(nbClients) + ", max " + std::string(nbMaxClients) + "\r\n")

#define RPL_MOTD(nick, motd) (std::string(":") + SERVER + " 372 " + std::string(nick) + " :" + std::string(motd) + "\r\n")
#define RPL_MOTDSTART(nick) (std::string(":") + SERVER + " 375 " + std::string(nick) + " :- " + SERVER + " Message of the day - \r\n")
#define RPL_ENDOFMOTD(nick) (std::string(":") + SERVER + " 376 " + std::string(nick) + " :End of /MOTD command.\r\n")

#define RPL_NOTOPIC(nick, channel) (std::string(":") + SERVER + " 331 " + std::string(nick) + " " + std::string(channel) + " :No topic is set\r\n")
#define RPL_TOPIC(nick, channel, topic) (std::string(":") + SERVER + " 332 " + std::string(nick) + " " + std::string(channel) + " :" + std::string(topic) + "\r\n")
#define RPL_TOPICWHOTIME(nick, channel, who, when) (std::string(":") + SERVER + " 333 " + std::string(nick) + " " + std::string(channel) + " " + std::string(who) + " " + std::string(when) + "\r\n")

#define ERR_BADCHANNELKEY(nick, channel) (std::string(":") + SERVER + " 475 " + std::string(nick) + " " + std::string(channel) + " :Cannot join channel (+k)\r\n")
#define ERR_INVITEONLYCHAN(nick, channel) (std::string(":") + SERVER + " 473 " + std::string(nick) + " " + std::string(channel) + " :Cannot join channel (+i)\r\n")
#define ERR_NICKNAMEINUSE(nick) (std::string(":") + SERVER + " 433 " + std::string(nick) + " " + std::string(nick) + " :Nickname is already in use\r\n")
#define ERR_NOTONCHANNEL(nick, channel) (std::string(":") + SERVER + " 442 " + std::string(nick) + " " + std::string(channel) + " :You're not on that channel\r\n")
#define ERR_NOSUCHCHANNEL(nick, channel) (std::string(":") + SERVER + " 403 " + std::string(nick) + " " + std::string(channel) + " :No such channel\r\n")
#define ERR_CHANOPRIVSNEEDED(nick, channel) (std::string(":") + SERVER + " 482 " + std::string(nick) + " " + std::string(channel) + " :You're not channel operator\r\n")

#define JOIN(nick, username, channel) (std::string(":") + std::string(nick) + "!" + std::string(username) + "@" + SERVER + " JOIN :" + std::string(channel) + "\r\n")

#endif
