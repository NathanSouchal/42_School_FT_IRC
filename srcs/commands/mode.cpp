#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <map>
#include "numerics.hpp"
#include "utils.hpp"

std::vector<std::string>	Server::parseModes(const std::string& src, Client *client)
{
	std::vector<std::string>	parsedModes, error;
	std::string	result;
	char	sign = 0;
	int		i = 0;

	while (src[i])
	{
		if (i == 10)
			return error;
		if (src[i] == '-' || src[i] == '+')
			break ;
		client->reply(ERR_UNKNOWNMODE(client->getNickname(), convertInString(src[i])));
		i++;
	}
	for (size_t j = i; j < src.size(); ++j)
	{
		if (j != 0 && (src[j] == '+' || src[j] == '-') && ((src[j - 1] == '+' || src[j - 1] == '-') || (j + 1 == src.size())))
			client->reply(ERR_UNKNOWNMODE(client->getNickname(), convertInString(src[j])));
		if (src[j] == '+')
			sign = '+';
		else if (src[j] == '-')
			sign = '-';
		else
		{
			if (src[j] != 'k' && src[j] != 'o' && src[j] != 'l' && src[j] != 'l' && src[j] != 'i' && src[j] != 't')
				client->reply(ERR_UNKNOWNMODE(client->getNickname(), convertInString(src[j])));
			else
			{
				result += sign;
				result += src[j];
				parsedModes.push_back(result);
				result.clear();
			}
		}
	}
	return parsedModes;
}

std::map<std::string, std::string>	Server::joinModesAndParams(const std::vector<std::string>& modes, const std::vector<std::string>& params, Client *client)
{
	std::map<std::string, std::string>	modesAndParams;
	size_t	paramCounter = 0;

	for (size_t i = 0; i < modes.size(); ++i)
	{
		if (modes[i][1] == 'k' || modes[i][1] == 'o' || modes[i][1] == 'l')
		{
			if (!params.size())
				client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
			else if (paramCounter > params.size() - 1)
				client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
			else
			{
				modesAndParams[modes[i]] = params[paramCounter];
				paramCounter++;
			}
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

	channel = parsedMessage[1];
	if (!findChannel(channel))
		return client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channel));
	Channel*	channelCopy = findChannel(channel);

	if (!channelCopy->findClientInChannel(client->getNickname()))
		return client->reply(ERR_NOTONCHANNEL(client->getNickname(), channel));
	//ici return modes actuels du channel
	//si pas operateur retourne tout sans les params, sinon retourne tout
	if (parsedMessage.size() == 2)
	{
		if (!channelCopy->findOperatorInChannel(client->getNickname()))
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
	if (!channelCopy->findOperatorInChannel(client->getNickname()))
		return client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channel));
	parsedModes = parseModes(parsedMessage[2], client);
	if (parsedModes.empty())
		return client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "MODE"));
	parsedMessage.erase(parsedMessage.begin(), parsedMessage.begin()+3);
	modesAndParams = joinModesAndParams(parsedModes, parsedMessage, client);
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
				if (it->second.empty())
					client->reply(ERR_INVALIDMODEPARAM(client->getNickname(), channel->getName(), "+k", ""));
				else
				{
					channel->setKey(it->second);
					modes += "+k";
					params.push_back(it->second);
				}
			}
			else if (it->first[1] == 'o' && !channel->findOperatorInChannel(it->second))
			{
				if (it->second.empty())
					client->reply(ERR_INVALIDMODEPARAM(client->getNickname(), channel->getName(), "+o", ""));
				else if (!findNickName(it->second))
					client->reply(ERR_NOSUCHNICK(client->getNickname(), it->second));
				else if (!channel->findClientInChannel(it->second))
					client->reply(ERR_USERNOTINCHANNEL(client->getNickname(), it->second, channel->getName()));
				else
				{
					channel->addChannelOperator(channel->findClientInChannel(it->second));
					modes += "+o";
					params.push_back(it->second);
				}
			}
			else if (it->first[1] == 'l')
			{
				if (it->second.empty())
					client->reply(ERR_INVALIDMODEPARAM(client->getNickname(), channel->getName(), "+l", ""));
				else
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
			else if (it->first[1] == 'o' && channel->findOperatorInChannel(it->second))
			{
				channel->deleteChannelOperator(channel->findClientInChannel(it->second));
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
	channel->sendMessageToAllClients(client, "MODE", modes, paramsStr);
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

//Normalement si mauvaise combinaison mode+param on rep ERR_INVALIDMODEPARAM au client, mais comment savoir quel mode associe a quel param est invalide ?
//pour l'instant erreur generique ERR_NEEDMOREPARAMS
