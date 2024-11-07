#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "utils.hpp"
#include "numerics.hpp"

void	Server::join(const std::string& message, Client *client)
{
	std::vector<std::string>	vecChannelNames;
	std::vector<std::string>	vecChannelKeys;
	std::string					temp;
	std::string					key;

	std::vector<std::string>	parameters = parseParams(message.substr(message.find(" ") + 1));
	if (parameters.size() > 2)
		return client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "JOIN"));
	std::string					channelNames = parameters[0];
	if (parameters.size() == 2)
	{
		std::string			channelKeys = parameters[1];
		std::stringstream	ss1(channelKeys);
		while (std::getline(ss1, temp, ','))
		{
			if (!temp.empty())
				vecChannelKeys.push_back(temp);
		}
	}
	std::stringstream			ss2(channelNames);
	while (std::getline(ss2, temp, ','))
	{
		if (!temp.empty())
		{
			if (!checkChannelName(temp, client))
				return ;
			vecChannelNames.push_back(temp);
		}
	}
	for (size_t i = 0; i < vecChannelNames.size(); ++i)
	{
		if (i < vecChannelKeys.size())
			key = vecChannelKeys[i];
		if (!checkAddClientToChannel(vecChannelNames[i], key, client))
			createChannel(vecChannelNames[i], key, client);
	}
}

bool	Server::checkAddClientToChannel(const std::string &name, const std::string &key, Client *client)
{
	for (size_t i = 0; i < serverChannels.size(); ++i)
	{
		if (serverChannels[i]->getName() == name)
		{
			if (serverChannels[i]->findClientInChannel(client->getNickname()))
			{
				client->reply(ERR_USERONCHANNEL(client->getNickname(), client->getNickname(), name));
				return true;
			}
			if (serverChannels[i]->getInviteOnly() && !serverChannels[i]->findInvitedClientInChannel(client->getNickname()))
			{
				client->reply(ERR_INVITEONLYCHAN(client->getNickname(), name));
				return true;
			}
			if (serverChannels[i]->getClientList().size() >= static_cast<size_t>(serverChannels[i]->getUserLimit()))
			{
				client->reply(ERR_CHANNELISFULL(client->getNickname(), serverChannels[i]->getName()));
				return true;
			}
			if (!serverChannels[i]->getKey().empty())
			{
				if (serverChannels[i]->getKey() == key)
					serverChannels[i]->addChannelClient(client);
				else
					client->reply(ERR_BADCHANNELKEY(client->getNickname(), name));
			}
			else
				serverChannels[i]->addChannelClient(client);
			serverChannels[i]->informUsersOnJoin(client);
			return true;
		}
	}
	return false;
}

void	Server::createChannel(const std::string &name, const std::string &key, Client *client)
{
	Channel *channel = new Channel(name);
	std::cout << "Channel " << name << " created !" << std::endl;
	channel->addChannelOperator(client);
	channel->addChannelClient(client);
	channel->setKey(key);
	serverChannels.push_back(channel);
}

bool	checkChannelName(std::string name, Client *client)
{
	if (name[0] == '&' || name[0] == '#')
		return true;
	client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), name));
	return false;
}
