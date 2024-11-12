#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "utils.hpp"
#include "numerics.hpp"

void	Server::sendTopic(Channel *channel, Client *client)
{
	if (!channel->findClientInChannel(client->getNickname()))
		return client->reply(ERR_NOTONCHANNEL(client->getNickname(), channel->getName()));
	if (channel->getChannelTopic().empty())
		return client->reply(RPL_NOTOPIC(client->getNickname(), channel->getName()));
	else
	{
		client->reply(RPL_TOPIC(client->getNickname(), channel->getName(), channel->getChannelTopic()));
		return client->reply(RPL_TOPICWHOTIME(client->getNickname(), channel->getName(), channel->getTopicCreator(), channel->getTopicCreationTime()));
	}
}

void	Server::topic(const std::string& message, Client *client)
{
	std::string	channel, topic;
	std::vector<std::string>	parsedParams = parseParams(message);
	if (parsedParams.size() == 1)
		return client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC"));
	channel = parsedParams[1];
	Channel*	channelCopy = findChannel(channel);
	if (!channelCopy)
		return client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channel));
	if (parsedParams.size() == 2)
		return sendTopic(channelCopy, client);
	if (parsedParams.size() == 3)
		topic = parsedParams[2];
	if (parsedParams.size() > 3)
		return client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC"));
	if (!channelCopy->findClientInChannel(client->getNickname()))
		return client->reply(ERR_NOTONCHANNEL(client->getNickname(), channel));
	if (!channelCopy->checkIfUserOperator(client->getNickname()) && !channelCopy->getModeT())
		return client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel));
	if (topic == ":" || topic == "::")
		channelCopy->setChannelTopic("");
	else
	{
		if (topic[0] == ':')
			topic = topic.substr(1);
		channelCopy->setChannelTopic(topic);
	}
	channelCopy->setTopicCreator(client->getNickname());
	channelCopy->setTopicCreationTime(convertInString(getTimestamp()));
	channelCopy->sendMessageToAllClients(client, "TOPIC", "", "");
}