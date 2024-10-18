/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:44:38 by tnicolau          #+#    #+#             */
/*   Updated: 2024/10/18 15:45:16 by tnicolau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::nickname(const std::string& message, Client *client)
{
	int			pos;
	std::string	nickname_sent;

	pos = message.find(" ");
	nickname_sent = message.substr(pos + 1);
	client->setNick(nickname_sent);
	if (!(client->getRealname().empty()) && !(client->getUsername().empty()))
		client->setTrueRegistration();
}

void	Server::user(const std::string& message, Client *client)
{
	int			pos;
	int			pos2;
	std::string	username_sent;
	std::string	realname_sent;

	pos = message.find(32);
	pos2 = (message.substr(pos + 1)).find(32);
	username_sent = message.substr(pos + 1, pos2);
	client->setUsername(username_sent);
	pos = message.find(":");
	realname_sent = message.substr(pos + 1);
	client->setRealname(realname_sent);
	std::cout << "username = " << client->getUsername() << " | realname = " << client->getRealname() << std::endl;
	if (!(client->getNickname().empty()))
		client->setTrueRegistration();
}

void	Server::motd(const std::string& message, Client *client)
{
	(void)message;
	client->reply(RPL_MOTD(client->getNickname(), "Coucouuuuuu c'est le message du jooouuuur"));
}

void	Server::checkCommand(const std::string& message, Client *current_client)
{
	std::string	command = message.substr(0, message.find(" "));
	(void)current_client;

	void(Server::*function_ptr[])(const std::string&, Client *) = {&Server::nickname, &Server::user, &Server::motd};
	std::string commands[] = {"NICK", "USER", "MOTD"};
	bool	found = false;

	for (size_t i = 0; i < commands->length() + 1; i++)
	{
		if (commands[i] == command)
		{
			(this->*function_ptr[i])(message, current_client);
			found = true;
			break ;
		}
	}
	if (!found)
		std::cerr << "Command " << message << " does not exist, sorry" << std::endl;
	return ;
}
