#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <map>
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

//normalement rep ERR_INVALIDMODEPARAM, mais comment savoir quel mode associe a quel paran est invalide ?
//pour l'instant erreur generique ERR_NEEDMOREPARAMS

bool	Server::checkModeParams(const std::vector<std::string>& modes, const std::vector<std::string>& params)
{
	size_t	paramCounter = 0;

	for (size_t i = 0; i < modes.size(); ++i)
	{
		if (((modes[i][1] == 'k' || modes[i][1] == 'l') && modes[i][0] == '+') || modes[i][1] == 'o')
		{
			if (paramCounter >= params.size())
				return false;
			paramCounter++;
		}
	}
	return paramCounter == params.size();
}

std::map<std::string, std::string>	Server::joinModesAndParams(const std::vector<std::string>& modes, const std::vector<std::string>& params)
{
	std::map<std::string, std::string>	modesAndParams;
	size_t	paramCounter = 0;

	for (size_t i = 0; i < modes.size(); ++i)
	{
		if (modes[i][1] == 'k' || modes[i][1] == 'o' || modes[i][1] == 'l')
		{
			modesAndParams[modes[i]] = params[paramCounter];
			paramCounter++;
		}
		else
			modesAndParams[modes[i]] = "";
	}
	return modesAndParams;
}

void	Server::mode(const std::string& message, Client *client)
{
	std::vector<std::string>	parsedMessage = parseParams(message);
	std::vector<std::string>	parsedModes;
	std::map<std::string, std::string>	modesAndParams;
	std::string	channel;

	if (parsedMessage.size() == 1)
		return client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
	channel = parsedMessage[1];
	if (!findChannel(channel))
		return client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channel));
	Channel*	channelCopy = findChannel(channel);

	if (!channelCopy->getChannelClient(client->getNickname()))
		return client->reply(ERR_NOTONCHANNEL(client->getNickname(), channel));
	//ici return modes actuels du channel
	//si pas operateur retourne tout sans les params, sinon retourne tout
	if (parsedMessage.size() == 2)
	{
		if (!channelCopy->getChannelOperator(client->getNickname()))
			return client->reply(RPL_CHANNELMODEIS1(client->getNickname(), channel, channelCopy->getActiveModes()));
		else
		{
			std::string	activeParams;
			if (!channelCopy->getKey().empty())
			{
				activeParams += channelCopy->getKey();
				activeParams += " ";
			}
			activeParams += convertInString(channelCopy->getUserLimit());
			return client->reply(RPL_CHANNELMODEIS2(client->getNickname(), channel, channelCopy->getActiveModes(), activeParams));
		}

	}
	if (!channelCopy->getChannelOperator(client->getNickname()))
		return client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel));
	parsedModes = parseModes(parsedMessage[2]);
	if (parsedModes.empty())
		return client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
	if (parsedMessage.size() >= 4)
	{
		parsedMessage.erase(parsedMessage.begin(), parsedMessage.begin()+3);
		if (!checkModeParams(parsedModes, parsedMessage))
			return client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
	}
	modesAndParams = joinModesAndParams(parsedModes, parsedMessage);
	for (std::map<std::string, std::string>::iterator it = modesAndParams.begin(); it != modesAndParams.end(); ++it)
		std::cout << it->first << ", " << it->second << std::endl;
	setNewModes(modesAndParams, channelCopy, client);
}

void	Server::setNewModes(std::map<std::string, std::string> modesAndParams, Channel* channel, Client* client)
{
	std::string	modes, paramsStr;
	std::vector<std::string>	params;

	for (std::map<std::string, std::string>::iterator it = modesAndParams.begin(); it != modesAndParams.end(); ++it)
	{
		if (it->first[0] == '+')
		{
			if (it->first[1] == 'i' && !channel->getInviteOnly())
			{
				channel->setInviteOnly();
				modes += "+i";
			}
			else if (it->first[1] == 't' && !channel->getModeT())
			{
				channel->setModeT();
				modes += "+t";
			}
			else if (it->first[1] == 'k')
			{
				channel->setKey(it->second);
				modes += "+k";
				params.push_back(it->second);
			}
			else if (it->first[1] == 'o' && !channel->getChannelOperator(it->second) && channel->getChannelClient(it->second))
			{
				channel->addChannelOperator(channel->getChannelClient(it->second));
				modes += "+o";
				params.push_back(it->second);
			}
			else if (it->first[1] == 'l')
			{
				char *end;
				long int	res = strtol(it->second.c_str(), &end, 10);
				if (!res || res > 2000)
					client->reply(ERR_INVALIDMODEPARAM(client->getNickname(), channel->getName(), "+l", it->second));
				else
				{
					channel->setUserLimit((int)res);
					modes += "+l";
					params.push_back(it->second);
				}
			}
		}
		else
		{
			if (it->first[1] == 'i' && channel->getInviteOnly())
			{
				channel->setInviteOnly();
				modes += "-i";
			}
			else if (it->first[1] == 't' && channel->getModeT())
			{
				channel->setModeT();
				modes += "-t";
			}
			else if (it->first[1] == 'k')
			{
				channel->setKey("");
				modes += "-k";
			}
			else if (it->first[1] == 'o' && channel->getChannelOperator(it->second))
			{
				channel->deleteChannelOperator(channel->getChannelClient(it->second));
				modes += "-o";
				params.push_back(it->second);
			}
			else if (it->first[1] == 'l')
			{
				channel->setUserLimit(10);
				modes += "-l";
			}
		}
	}
	//JOIN params
	for (size_t i = 0; i < params.size(); ++i)
	{
		if (paramsStr.empty())
			paramsStr += params[i];
		else
		{
			paramsStr += " ";
			paramsStr += params[i];
		}
	}
	channel->sendMessageToAllClients("MODE", modes, paramsStr);
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

//Si mode deja actif ou inactif, on ignore
