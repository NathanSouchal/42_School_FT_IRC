#include "Server.hpp"
#include "Client.hpp"
#include "numerics.hpp"
#include "utils.hpp"

void	Server::privmsg(const std::string& message, Client *client)
{
	std::vector<std::string>	parameters = parseParams(message.substr(message.find(" ")));
	std::vector<std::string>	targets;
	std::string					msgToSend, temp;

	if (parameters.size() != 2)
	{
		if (parameters.size() > 2)
			client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "JOIN"));
		else if ((parameters.size() == 1 && parameters[0][0] == ':') || parameters.size() == 0)
			client->reply(ERR_NORECIPIENT(client->getNickname(), "PRIVMSG"));
		else if (parameters.size() == 1)
			client->reply(ERR_NOTEXTTOSEND(client->getNickname()));
		return ;
	}
	msgToSend = parameters[1];
	std::stringstream			ss(parameters[0]);
	while (std::getline(ss, temp, ','))
	{
		if (!temp.empty())
			targets.push_back(temp);
	}
	for (size_t i = 0; i < targets.size(); ++i)
	{
		if (targets[i][0] == '&' || targets[i][0] == '#') {}
			//messageToChannelFunction
		else {}
			//messageToUserFunction
	}
}
