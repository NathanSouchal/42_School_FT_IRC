#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "numerics.hpp"
#include "utils.hpp"

std::vector<std::string>	Server::parseModes(const std::string& src, Client *client)
{
	std::vector<std::string>	parsedModes, error;
	std::string	result;
	char	sign = 0;

	if (src[0] != '-' && src[0] != '+')
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
		return error;
	}
	sign = src[0];
	for (size_t i = 1; i < src.size(); ++i)
	{
		if ((src[i] == '+' || src[i] == '-') && (src[i - 1] == '+' || src[i - 1] == '-'))
		{
			client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
			return error;
		}
		if (src[i] == '+')
			sign = '+';
		else if (src[i] == '-')
			sign = '-';
		else
		{
			result += sign;
			result += src[i];
			std::cout << "res : " << result << std::endl;
			parsedModes.push_back(result);
			result.clear();
		}
	}
	return parsedModes;
}

void	Server::parseParameters(const std::string& src, Client *client)
{
	std::cout << src << std::endl;
	(void)client;
}

void	Server::mode(const std::string& message, Client *client)
{
	std::cout << "messsage:: " << message << std::endl;
	std::vector<std::string>	parsedMessage = parseParams(message);
	std::vector<std::string>	parsedModes, parsedMessage;
	std::string	channel;
	if (parsedMessage.size() == 1 || parsedMessage.size() > 4)
		return client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
	channel = parsedMessage[1];
	if (!findChannel(channel))
		return client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channel));
	Channel*	channelCopy = findChannel(channel);

	if (!channelCopy->getChannelClient(client->getNickname()))
		return client->reply(ERR_NOTONCHANNEL(client->getNickname(), channel));
	if (parsedMessage.size() == 2)
		return client->reply(RPL_CHANNELMODEIS(client->getNickname(), channel, "+k", "password"));
	if (parsedMessage.size() == 3)
		parsedModes = parseModes(parsedMessage[2], client);
	if (parsedModes.empty())
		return client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
	if (parsedMessage.size() == 4)
		parseParameters(parsedMessage[3], client);
}


//MODE
//->liste des modes i t k o l

//MODE #channel ou MODE nickname
//->modes actuels du channel ou user

//MODE <cible> +mode
//->active le mode (si il n,a pas besoin de parametre)

//MODE <cible> +mode+mode-mode parametre
//MODE #channel +mode-mode user1 user2
