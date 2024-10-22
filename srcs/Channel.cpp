/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:40:01 by nsouchal          #+#    #+#             */
/*   Updated: 2024/10/22 17:05:23 by tnicolau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"

Channel::Channel(const std::string &name): _name(name) {}

const std::string	&Channel::getName()
{
	return (_name);
}

const std::string	&Channel::getKey()
{
	return (_key);
}

bool	Channel::getInviteOnly()
{
	return _inviteOnly;
}

void	Channel::setInviteOnly()
{
	_inviteOnly = !_inviteOnly;
}

void	Channel::setKey(const std::string &key)
{
	_key = key;
}

void	Channel::addChannelClient(Client *client, const std::string& name)
{
	std::vector<Client*>::iterator it = std::find(channelClients.begin(), channelClients.end(), client);
	if (it != channelClients.end())
		return ;
	channelClients.push_back(client);
	client->reply(JOIN(client->getNickname(), client->getUsername(), name));
}

void	Channel::addChannelOperator(Client *client)
{
	std::vector<Client*>::iterator it = std::find(channelOperators.begin(), channelOperators.end(), client);
	if (it != channelOperators.end())
		return ;
	channelOperators.push_back(client);
	std::cout << "Added channel Operator " << client->getNickname() << std::endl;
}

Client*		Channel::getChannelOperator(const std::string& nickname)
{
	for (std::vector<Client*>::iterator it = channelOperators.begin(); it != channelOperators.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
			return *it;
	}
	return NULL;
}

Client*		Channel::getChannelClient(const std::string& nickname)
{
	for (std::vector<Client*>::iterator it = channelClients.begin(); it != channelClients.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
			return *it;
	}
	return NULL;
}

std::string		Channel::getChannelTopic()
{
	return this->_channelTopic;
}

std::string		Channel::getTopicCreator()
{
	return this->_topicCreator;
}

std::string		Channel::getTopicCreationTime()
{
	return this->_topicCreationTime;
}

bool		Channel::getModeT()
{
	return this->_modeT;
}

void		Channel::setModeT()
{
	this->_modeT = !_modeT;
}

void		Channel::setChannelTopic(const std::string& topic)
{
	this->_channelTopic = topic;
}

void		Channel::setTopicCreator(const std::string& creator)
{
	this->_topicCreator = creator;
}

void		Channel::setTopicCreationTime(const std::string& creationTime)
{
	this->_topicCreationTime = creationTime;
}
