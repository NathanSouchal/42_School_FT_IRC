#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "utils.hpp"
#include "numerics.hpp"

void	Server::topic(const std::string& message, Client *client)
{
	std::cout << "message : " << message << std::endl;
	size_t		pos = message.find(":");
	if (pos == std::string::npos)
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC"));
		return ;
	}
	std::string	param = message.substr(pos + 1);
	std::string	channel, topic;
	std::vector<std::string>	parsedParams = parseParams(param);
	channel = parsedParams[0];
	if (parsedParams.size() == 2)
		topic = parsedParams[1];
	if (parsedParams.size() > 2)
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC"));
		return ;
	}
	if (!findChannel(channel))
		client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channel));
	else
	{
		Channel*	channelCopy = findChannel(channel);

		if (!channelCopy->findClientInChannel(client->getNickname()))
			client->reply(ERR_NOTONCHANNEL(client->getNickname(), channel));
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
		else
		{
			if (!channelCopy->getModeT() && !channelCopy->findOperatorInChannel(client->getNickname()))
				client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel));
			//si -t non-active les non-operateurs peuvent pas mofifier le topic
			else
			{
				if (topic == ":")
					channelCopy->setChannelTopic("");
				else
				{
					if (topic[0] == ':')
						topic = topic.substr(1);
					channelCopy->setChannelTopic(topic);
				}
				channelCopy->setTopicCreator(client->getNickname());
				channelCopy->setTopicCreationTime(convertInString(getTimestamp()));
				//ici envoyer le nouveau topic a tous les membres du channel
				channelCopy->sendMessageToAllClients("TOPIC");
			}
		}
	}
	std::cout << "channel : " << channel << "|" << std::endl;
}
