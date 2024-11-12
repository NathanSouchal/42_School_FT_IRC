#include "Server.hpp"
#include "Client.hpp"
#include "numerics.hpp"
#include "utils.hpp"

void	Server::user(const std::string& message, Client *client)
{
	std::vector<std::string>	parsedMessage = parseParams(message);

	if (!client->getUsername().empty() && !client->getRealname().empty())
		return client->reply(ERR_ALREADYREGISTERED(client->getNickname(), "USER"));
	if (parsedMessage.size() < 5)
		return client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "USER"));
	client->setUsername(parsedMessage[1]);
	client->setRealname(parsedMessage[4]);
	if (!client->checkEmptyNickname())
		client->setTrueRegistration();
}
