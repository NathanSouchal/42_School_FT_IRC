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
	std::cout << "CHANNELS : " << channelNames << std::endl;
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
				{
					serverChannels[i]->addChannelClient(client);
					serverChannels[i]->replySuccessfullJoin(client);
				}
				else
					client->reply(ERR_BADCHANNELKEY(client->getNickname(), name));
			}
			else
			{
				serverChannels[i]->addChannelClient(client);
				serverChannels[i]->replySuccessfullJoin(client);
			}
			return true;
		}
	}
	return false;
}

void	Server::createChannel(const std::string &name, const std::string &key, Client *client)
{
	Channel *channel = new Channel(name);
	std::cout << "Channel " << name << " created !" << std::endl;
	channel->addChannelClient(client);
	std::cout << client->getNickname() << " added to channel " << name << " !" << std::endl;
	channel->addChannelOperator(client);
	channel->setKey(key);
	serverChannels.push_back(channel);
	channel->replySuccessfullJoin(client);
}

bool	checkChannelName(std::string name, Client *client)
{
	if (name[0] == '&' || name[0] == '#')
		return true;
	client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), name));
	return false;
}
