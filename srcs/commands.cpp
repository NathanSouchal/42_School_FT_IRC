/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:44:38 by tnicolau          #+#    #+#             */
/*   Updated: 2024/10/21 15:37:48 by tnicolau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

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
	if (!(client->getNickname().empty()))
		client->setTrueRegistration();
}

void	Server::motd(const std::string& message, Client *client)
{
	(void)message;
	client->reply(RPL_MOTDSTART(client->getNickname()));
	client->reply(RPL_MOTD(client->getNickname(), "Coucouuuuuu c'est le message du jooouuuur"));
	client->reply(RPL_ENDOFMOTD(client->getNickname()));
}

void	Server::lusers(const std::string& message, Client *client)
{
	(void)message;
	(void)client;

}

bool	Server::checkAddClientToChannel(const std::string &name, const std::string &key, Client *client)
{
	for (size_t i = 0; i < serverChannels.size(); ++i)
	{
		if (serverChannels[i]->getName() == name)
		{
			if (!serverChannels[i]->getKey().empty())
			{
				if (serverChannels[i]->getKey() == key)
				{
					serverChannels[i]->addChannelClient(client);
				}
				else
					client->reply(ERR_BADCHANNELKEY(client->getNickname(), name));
			}
			else
				serverChannels[i]->addChannelClient(client);
			return true;
		}
	}
	return false;
}

void	Server::join(const std::string& message, Client *client)
{
	std::string 				parameters = message.substr(message.find(" ") + 1);
	std::string 				channelNames = parameters.substr(0, parameters.find(" "));
	std::string 				channelKeys = parameters.substr(parameters.find(" ") + 1);
	std::vector<std::string>	vecChannelNames;
	std::vector<std::string>	vecChannelKeys;
	std::stringstream			ss(channelNames);
	std::string					temp;
	std::string					key;

	(void)client;
	while (std::getline(ss, temp, ','))
	{
		if (!temp.empty())
			vecChannelNames.push_back(temp);
	}
	temp.clear();
	ss.clear();
	ss.str(channelKeys);
	while (std::getline(ss, temp, ','))
	{
		if (!temp.empty())
			vecChannelKeys.push_back(temp);
	}

	//
	for (size_t i = 0; i < vecChannelNames.size(); ++i)
	{
		if (i < vecChannelKeys.size())
			key = vecChannelKeys[i];
		if (!checkAddClientToChannel(vecChannelNames[i], key, client))
		{
			
		}
	}
}

void	Server::privmsg(const std::string& message, Client *client)
{
	(void)message;
	(void)client;
}

void	Server::kick(const std::string& message, Client *client)
{
	(void)message;
	(void)client;
}

void	Server::invite(const std::string& message, Client *client)
{
	(void)message;
	(void)client;
}

void	Server::topic(const std::string& message, Client *client)
{
	(void)message;
	(void)client;
}

void	Server::mode(const std::string& message, Client *client)
{
	(void)message;
	(void)client;
}

void	Server::checkCommand(const std::string& message, Client *current_client)
{
	std::string	command = message.substr(0, message.find(" "));
	(void)current_client;

	void(Server::*function_ptr[])(const std::string&, Client *) = {&Server::nickname, &Server::user, &Server::motd,\
	&Server::lusers, &Server::join, &Server::privmsg, &Server::kick, &Server::invite, &Server::topic,&Server::mode};
	std::string commands[] = {"NICK", "USER", "MOTD", "LUSERS", "JOIN", "PRIVMSG", "KICK", "INVITE" "TOPIC", "MODE"};
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
