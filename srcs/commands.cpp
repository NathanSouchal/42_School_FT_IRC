/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:44:38 by tnicolau          #+#    #+#             */
/*   Updated: 2024/10/22 16:30:45 by tnicolau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "utils.hpp"

void	Server::nickname(const std::string& message, Client *client)
{
	size_t		pos = message.find(" ");
	std::string	nickname_sent;

	if (pos == std::string::npos)
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "NICK"));
	else
	{
		nickname_sent = message.substr(pos + 1);
		if (findNickName(nickname_sent))
		{
			client->reply(ERR_NICKNAMEINUSE(nickname_sent));
			return ;
		}
		client->setNick(nickname_sent);
		if (!(client->getRealname().empty()) && !(client->getUsername().empty()))
			client->setTrueRegistration();
	}
}

void	Server::user(const std::string& message, Client *client)
{
	size_t		pos = message.find(32);
	std::string	username_sent;
	std::string	realname_sent;

	if (pos == std::string::npos)
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "USER"));
	else
	{
		size_t		pos2 =(message.substr(pos + 1)).find(32);

		if (pos2 == std::string::npos)
			client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "USER"));
		else
		{
			username_sent = message.substr(pos + 1, pos2);
			client->setUsername(username_sent);
			pos = message.find(":");
			if (pos == std::string::npos)
				client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "USER"));
			else
			{
				realname_sent = message.substr(pos + 1);
				client->setRealname(realname_sent);
				if (!(client->getNickname().empty()))
					client->setTrueRegistration();
			}
		}
	}
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

	client->reply(RPL_LUSERCLIENT(client->getNickname(), convertInString(clients.size()), "0"));
	client->reply(RPL_LUSEROP(client->getNickname(), "1"));
	client->reply(RPL_LUSERUNKNOWN(client->getNickname(), convertInString(clients.size() - _nbUsers)));
	client->reply(RPL_LUSERCHANNELS(client->getNickname(), convertInString(serverChannels.size())));
	client->reply(RPL_LUSERME(client->getNickname(), convertInString(clients.size())));
	client->reply(RPL_LOCALUSERS(client->getNickname(), convertInString(clients.size()), convertInString(_nbMaxClients)));
	client->reply(RPL_GLOBALUSERS(client->getNickname(), convertInString(clients.size()), convertInString(_nbMaxClients)));
}

bool	Server::checkAddClientToChannel(const std::string &name, const std::string &key, Client *client)
{
	for (size_t i = 0; i < serverChannels.size(); ++i)
	{
		if (serverChannels[i]->getName() == name)
		{
			if (serverChannels[i]->getInviteOnly())
			{
				client->reply(ERR_INVITEONLYCHAN(client->getNickname(), name));
				return true;
			}
			if (!serverChannels[i]->getKey().empty())
			{
				if (serverChannels[i]->getKey() == key)
					serverChannels[i]->addChannelClient(client, name);
				else
					client->reply(ERR_BADCHANNELKEY(client->getNickname(), name));
			}
			else
				serverChannels[i]->addChannelClient(client, name);
			return true;
		}
	}
	return false;
}

void	Server::createChannel(const std::string &name, const std::string &key, Client *client)
{
	Channel *channel = new Channel(name);
	std::cout << "Channel " << name << " created !" << std::endl;
	channel->addChannelClient(client, name);
	std::cout << client->getNickname() << " added to channel " << name << " !" << std::endl;
	channel->addChannelOperator(client);
	channel->setKey(key);

	serverChannels.push_back(channel);
}

void	Server::join(const std::string& message, Client *client)
{
	std::vector<std::string>	vecChannelNames;
	std::vector<std::string>	vecChannelKeys;
	std::string					temp;
	std::string					key;

	std::string					parameters = message.substr(message.find(" ") + 1);
	size_t 		pos = parameters.find(" ");
	std::string					channelNames;
	if (pos == std::string::npos)
		channelNames = parameters;
	else
	{
		channelNames = parameters.substr(0, pos);
		std::string					channelKeys = parameters.substr(pos + 1);
		std::stringstream			ss1(channelKeys);
		while (std::getline(ss1, temp, ','))
		{
			if (!temp.empty())
				vecChannelKeys.push_back(temp);
		}
		std::cout << "KEYS : " << channelKeys << std::endl;
	}
	std::stringstream			ss2(channelNames);
	while (std::getline(ss2, temp, ','))
	{
		if (!temp.empty())
			vecChannelNames.push_back(temp);
	}
	for (size_t i = 0; i < vecChannelNames.size(); ++i)
	{
		if (i < vecChannelKeys.size())
			key = vecChannelKeys[i];
		if (!checkAddClientToChannel(vecChannelNames[i], key, client))
			createChannel(vecChannelNames[i], key, client);
	}
	std::cout << "CHANNELS : " << channelNames << std::endl;
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
	size_t 		pos = message.find(" ");
	std::string	command = message.substr(pos + 1);
	size_t 		pos2 = command.find(" ");
	std::string	channel, topic;

	if (pos2 == std::string::npos)
		channel = command;
	else
	{
		channel = command.substr(0, pos);
		topic = command.substr(pos2 + 1);
		std::cout << "topic : " << topic << std::endl;
	}
	if (!findChannel(channel))
		client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channel));
	//si -t active les non-operateurs peuvent aussi mofifier le topic
	else if (!findUserInChannel(client->getNickname(), channel))
		client->reply(ERR_NOTONCHANNEL(client->getNickname(), channel));
	else
	{
		Channel*	channelCopy = findChannel(channel);

		if (topic.empty())
		{
			// Demande le topic actuel du canal
			if (channelCopy->getChannelTopic().empty())
				client->reply(RPL_NOTOPIC(client->getNickname(), channel));
			else
			{
				client->reply(RPL_TOPIC(client->getNickname(), channel, channelCopy->getChannelTopic()));
				client->reply(RPL_TOPICWHOTIME(client->getNickname(), channel, channelCopy->getTopicCreator(), channelCopy->getTopicCreationTime()));
			}
		}
		//else
		// TOPIC #test :Nouveau topic : Tente de changer le topic du canal #test.
		// TOPIC #test : : Tente d'effacer le topic du canal #test.

	}
	std::cout << "channel : " << channel << std::endl;
}

void	Server::mode(const std::string& message, Client *client)
{
	(void)message;
	(void)client;
}

void	Server::checkCommand(const std::string& message, Client *current_client)
{
	size_t 		pos = message.find(" ");
	std::string	command;

	if (pos == std::string::npos)
		command = message;
	else
		command = message.substr(0, pos);
	void(Server::*function_ptr[])(const std::string&, Client *) = {&Server::nickname, &Server::user, &Server::motd,\
	&Server::lusers, &Server::join, &Server::privmsg, &Server::kick, &Server::invite, &Server::topic, &Server::mode};
	std::string commands[] = {"NICK", "USER", "MOTD", "LUSERS", "JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE"};
	bool	found = false;

	for (size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
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
