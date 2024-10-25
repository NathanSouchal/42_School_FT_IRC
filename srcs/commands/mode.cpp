#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "numerics.hpp"
#include "utils.hpp"

std::vector<std::string>	Server::parseModes(const std::string& src)
{
	std::vector<std::string>	parsedModes, error;
	std::string	result;
	char	sign = 0;

	if (src[0] != '-' && src[0] != '+')
		return error;
	sign = src[0];
	for (size_t i = 1; i < src.size(); ++i)
	{
		if ((src[i] == '+' || src[i] == '-') && (src[i - 1] == '+' || src[i - 1] == '-'))
			return error;
		if (src[i] == '+')
			sign = '+';
		else if (src[i] == '-')
			sign = '-';
		else
		{
			if (src[i] != 'k' && src[i] != 'o' && src[i] != 'l' && src[i] != 'l' && src[i] != 'i' && src[i] != 't')
				return error;
			result += sign;
			result += src[i];
			parsedModes.push_back(result);
			result.clear();
		}
	}
	return parsedModes;
}

bool	Server::checkModeParams(const std::vector<std::string>& modes, const std::vector<std::string>& params)
{
	size_t	paramCounter = 0;

	for (size_t i = 0; i < modes.size(); ++i)
	{
		if (modes[i][1] == 'k' || modes[i][1] == 'o' || modes[i][1] == 'l')
		{
			if (paramCounter >= params.size())
				return false;
			paramCounter++;
		}
	}
	return paramCounter == params.size();
}

void	Server::mode(const std::string& message, Client *client)
{
	std::vector<std::string>	parsedMessage = parseParams(message);
	std::vector<std::string>	parsedModes;
	std::string	channel;
	if (parsedMessage.size() == 1)
		return client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
	channel = parsedMessage[1];
	if (!findChannel(channel))
		return client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channel));
	Channel*	channelCopy = findChannel(channel);

	if (!channelCopy->getChannelClient(client->getNickname()))
		return client->reply(ERR_NOTONCHANNEL(client->getNickname(), channel));
	if (parsedMessage.size() == 2)
		return client->reply(RPL_CHANNELMODEIS(client->getNickname(), channel, "+k", "password"));
	if (parsedMessage.size() >= 3)
		parsedModes = parseModes(parsedMessage[2]);
	if (parsedModes.empty())
		return client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
	if (parsedMessage.size() >= 4)
	{
		parsedMessage.erase(parsedMessage.begin(), parsedMessage.begin()+3);
		if (!checkModeParams(parsedModes, parsedMessage))
			return client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
	}
}


//MODE
//->liste des modes i t k o l

//k -> password
//o -> nickname
//l -> number

//MODE #channel ou MODE nickname
//->modes actuels du channel ou user

//MODE <cible> +mode
//->active le mode (si il n,a pas besoin de parametre)

//MODE <cible> +mode+mode-mode parametre
//MODE #channel +mode-mode user1 user2
