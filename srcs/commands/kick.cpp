#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "numerics.hpp"
#include "utils.hpp"

void	Server::kick(const std::string& message, Client *client)
{
	std::vector<std::string>	parsedMessage = parseParams(message);
	std::string	channel;

	channel = parsedMessage[1];
	if (!findChannel(channel))
		return client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channel));
	Channel*	channelCopy = findChannel(channel);

	//check if user is in channel
	if (!channelCopy->findClientInChannel(client->getNickname()))
		return client->reply(ERR_NOTONCHANNEL(client->getNickname(), channel));
	//check if user is operator
	if (!channelCopy->findOperatorInChannel(client->getNickname()))
		return client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel));
	//not enough params
	if (parsedMessage.size() < 2)
		return client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "KICK"));
	//check nickname
	if (!channelCopy->findClientInChannel(parsedMessage[2]))
		return client->reply(ERR_USERNOTINCHANNEL(client->getNickname(), parsedMessage[2], channel));
	//envoi du message a tous les users meme celui kick
	if (parsedMessage.size() >= 4)
		channelCopy->sendMessageToAllClients("KICK", parsedMessage[2], parsedMessage[3]);
	else
		channelCopy->sendMessageToAllClients("KICK", parsedMessage[2], "");

	if (channelCopy->findOperatorInChannel(parsedMessage[2]))
		channelCopy->deleteChannelOperator(channelCopy->findOperatorInChannel(parsedMessage[2]));
	channelCopy->deleteChannelClient(channelCopy->findClientInChannel(parsedMessage[2]));

	//KICK #channel user1,user2,user3 :reason
	//possible mais le traitement doit etre fait un par un
}
