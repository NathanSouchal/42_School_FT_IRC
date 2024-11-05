#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "numerics.hpp"
#include "utils.hpp"

std::vector<std::string>	Server::parseUsers(const std::string& src)
{
	std::vector<std::string>	result;

	std::string					buffer;
	std::string					str = src;
	size_t 						pos = src.find(",");
	std::stringstream			ss(str);
	if (pos == std::string::npos)
		result.push_back(src);
	else
	{
		while (std::getline(ss, buffer, ','))
		{
			if (!buffer.empty())
				result.push_back(buffer);
		}
	}
	return result;
}

void	Server::kick(const std::string& message, Client *client)
{
	std::vector<std::string>	parsedMessage = parseParams(message);
	std::vector<std::string>	parsedUsers;
	std::string	channel;

	channel = parsedMessage[1];
	if (!findChannel(channel))
		return client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channel));
	Channel*	channelCopy = findChannel(channel);

	//check if user is in channel
	if (!channelCopy->findClientInChannel(client->getNickname()))
		return client->reply(ERR_NOTONCHANNEL(client->getNickname(), channel));
	//check if user is operator
	if (!channelCopy->checkIfUserOperator(client->getNickname()))
		return client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel));
	//not enough params
	if (parsedMessage.size() < 2)
		return client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "KICK"));
	//check nickname
	parsedUsers = parseUsers(parsedMessage[2]);
	for (size_t i = 0; i < parsedUsers.size(); ++i)
	{
		if (!channelCopy->findClientInChannel(parsedUsers[i]))
			client->reply(ERR_USERNOTINCHANNEL(client->getNickname(), parsedUsers[i], channel));
		else
		{
			//envoi du message a tous les users meme celui kick
			if (parsedMessage.size() >= 4)
				channelCopy->sendMessageToAllClients(client, "KICK", parsedUsers[i], parsedMessage[3]);
			else
				channelCopy->sendMessageToAllClients(client, "KICK", parsedUsers[i], "");

			if (channelCopy->findOperatorInChannel(parsedUsers[i]))
				channelCopy->deleteChannelOperator(channelCopy->findOperatorInChannel(parsedUsers[i]));
			channelCopy->deleteChannelClient(channelCopy->findClientInChannel(parsedUsers[i]));
			client->getServerRef().deleteChannel(channelCopy);
		}
	}
}
