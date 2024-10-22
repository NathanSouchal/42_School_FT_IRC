/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:40:01 by nsouchal          #+#    #+#             */
/*   Updated: 2024/10/22 10:18:19 by tnicolau         ###   ########.fr       */
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
	std::vector<Client*>::iterator it = std::find(channelClients.begin(), channelClients.end(), client);
	if (it != channelClients.end())
		return ;
	channelOperators.push_back(client);
}
