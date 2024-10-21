/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouchal <nsouchal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:40:01 by nsouchal          #+#    #+#             */
/*   Updated: 2024/10/21 16:13:26 by nsouchal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"

Channel::Channel(const std::string &name): _name(name) {}

const std::string   &Channel::getName()
{
    return (_name);
}

const std::string   &Channel::getKey()
{
    return (_key);
}

const bool  Channel::getInviteOnly()
{
    return _inviteOnly;
}

void    Channel::setInviteOnly()
{
    _inviteOnly = !_inviteOnly;
}

void    Channel::setKey(const std::string &key)
{
    _key = key;
}

void    Channel::addChannelClient(Client *client)
{
    channelClients.push_back(client);
}

void    Channel::addChannelOperator(Client *client)
{
    channelOperators.push_back(client);
}