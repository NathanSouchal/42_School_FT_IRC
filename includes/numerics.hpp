#ifndef NUMERICS_HPP
# define NUMERICS_HPP

# include "Server.hpp"

#define SERVER "FT_IRC"
#define NETWORKNAME "hier c, aujourd'hui c++"
#define ERR_NEEDMOREPARAMS(source, command) (std::string(":") + SERVER + " 461 " + std::string(source) + " " + std::string(command) + " :Not enough parameters\r\n")
#define ERR_TOOMANYPARAMS(source, command) (std::string(":") + SERVER + " 461 " + std::string(source) + " " + std::string(command) + " :Too many parameters\r\n")
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
#define RPL_NAMREPLY(nick, channel, user_list) (std::string(":") + SERVER + " 353 " + std::string(nick) + " = " + std::string(channel) + " :" + std::string(user_list) + "\r\n")
#define RPL_ENDOFNAMES(nick, channel) (std::string(":") + SERVER + " 366 " + std::string(nick) + " " + std::string(channel) + " :" + "End of /NAMES list\r\n")

#define RPL_UMODEIS(nick, modes) (std::string(":") + SERVER + " 221 " + std::string(nick) + " :" + std::string(modes) + "\r\n")
#define RPL_CHANNELMODEIS1(nick, channel, modes) (std::string(":") + SERVER + " 324 " + std::string(nick) + " " + std::string(channel) + " " + std::string(modes) + "\r\n")
#define RPL_CHANNELMODEIS2(nick, channel, modes, params) (std::string(":") + SERVER + " 324 " + std::string(nick) + " " + std::string(channel) + " " + std::string(modes) + " " + std::string(params) + "\r\n")
#define ERR_UNKNOWNMODE(nick, mode) (std::string(":") + SERVER + " 472 " + std::string(nick) + " " + std::string(mode) + " :is unknown mode char to me\r\n")
#define ERR_INVALIDMODEPARAM(nick, channel, mode, params) (std::string(":") + SERVER + " 696 " + std::string(nick) + " " + std::string(channel) + " " + std::string(mode) + " " + std::string(params) + " :Invalid channel mode parameter\r\n")

#define ERR_BADCHANNELKEY(nick, channel) (std::string(":") + SERVER + " 475 " + std::string(nick) + " " + std::string(channel) + " :Cannot join channel (+k)\r\n")
#define ERR_CHANNELISFULL(nick, channel) (std::string(":") + SERVER + " 471 " + std::string(nick) + " " + std::string(channel) + " :Cannot join channel (+l)\r\n")
#define ERR_INVITEONLYCHAN(nick, channel) (std::string(":") + SERVER + " 473 " + std::string(nick) + " " + std::string(channel) + " :Cannot join channel (+i)\r\n")
#define ERR_NICKNAMEINUSE(nick) (std::string(":") + SERVER + " 433 " + std::string(nick) + " " + std::string(nick) + " :Nickname is already in use\r\n")
#define ERR_ERRONEUSNICKNAME(nick) (std::string(":") + SERVER + " 432 " + std::string(nick) + " " + std::string(nick) + " :Erroneous nickname\r\n")
#define ERR_NOTONCHANNEL(nick, channel) (std::string(":") + SERVER + " 442 " + std::string(nick) + " " + std::string(channel) + " :You're not on that channel\r\n")
#define ERR_NOSUCHCHANNEL(nick, channel) (std::string(":") + SERVER + " 403 " + std::string(nick) + " " + std::string(channel) + " :No such channel\r\n")
#define ERR_USERNOTINCHANNEL(nick, str, channel) (std::string(":") + SERVER + " 441 " + std::string(nick) + " " + std::string(str) + " " + std::string(channel) + " :They aren't on that channel\r\n")
#define ERR_CHANOPRIVSNEEDED(nick, channel) (std::string(":") + SERVER + " 482 " + std::string(nick) + " " + std::string(channel) + " :You're not channel operator\r\n")
#define ERR_USERONCHANNEL(nick, user, channel) (std::string(":") + SERVER + " 443 " + std::string(nick) + " " + std::string(user) + " " + std::string(channel) + " :is already on channel\r\n")
#define RPL_INVITING(nick, user, channel) (std::string(":") + SERVER + " 341 " + std::string(nick) + " " + std::string(user) + " " + std::string(channel) + "\r\n")

#define ERR_NORECIPIENT(nick, command) (std::string(":") + SERVER + " 411 " + std::string(nick) + " :No recipient given " + std::string(command) + "\r\n")
#define ERR_NOTEXTTOSEND(nick) (std::string(":") + SERVER + " 412 " + std::string(nick) + " :No text to send\r\n")
#define ERR_NOSUCHNICK(nick, nickDestination) (std::string(":") + SERVER + " 401 " + std::string(nick) + " " + std::string(nickDestination) + " " + ":No such nick/channel\r\n")
#define ERR_CANNOTSENDTOCHAN(nick, channel) (std::string(":") + SERVER + " 404 " + std::string(nick) + " " + std::string(channel) + " " + ":Cannot send to channel\r\n")
#define ERR_UNKNOWNCOMMAND(nick, command) (std::string(":") + SERVER + " 421 " + std::string(nick) + " " + std::string(command) + " :Unknown command\r\n")
#define ERR_NOMOTD(nick) (std::string(":") + SERVER + " 422 " + std::string(nick) + " :MOTD File is missing\r\n")

#define JOIN(nick, username, ip, channel) (std::string(":") + std::string(nick) + "!" + std::string(username) + "@" + std::string(ip) + " JOIN :" + std::string(channel) + "\r\n")
#define TOPIC(nick, username, channel, ip, topic) (std::string(":") + std::string(nick) + "!" + std::string(username) + "@" + std::string(ip) + " TOPIC " + std::string(channel) + " :" + std::string(topic) + "\r\n")
#define MODE(nick, username, ip, channel, modes, params) (std::string(":") + std::string(nick) + "!" + std::string(username) + "@" + std::string(ip) + " MODE " + std::string(channel) + " " + std::string(modes) + " " + std::string(params) + "\r\n")
#define KICK(nick, username, ip, channel, target, reason) (std::string(":") + std::string(nick) + "!" + std::string(username) + "@" + std::string(ip) + " KICK " + std::string(channel) + " " + std::string(target) + " " + std::string(reason) + "\r\n")
#define PART(nick, username, ip, channel, reason) (std::string(":") + std::string(nick) + "!" + std::string(username) + "@" + std::string(ip) + " PART " + std::string(channel) + " :" + std::string(reason) + "\r\n")

#endif
