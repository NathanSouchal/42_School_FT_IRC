#include "Server.hpp"
#include "Client.hpp"
#include "numerics.hpp"

void	Server::nickname(const std::string& message, Client *client)
{
	size_t		pos = message.find(" ");
	std::string	nickname_sent;

	if (pos == std::string::npos)
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "NICK"));
	else
	{
		nickname_sent = message.substr(pos + 1);
		if (nickname_sent.size() > 9 || nickname_sent.find(",") != std::string::npos || nickname_sent.empty())
			return client->reply(ERR_ERRONEUSNICKNAME(nickname_sent));
		if (findNickName(nickname_sent))
			return client->reply(ERR_NICKNAMEINUSE(nickname_sent));
		if (!(client->getRealname().empty()) && !(client->getUsername().empty()) && client->getNickname() == "*")
			client->setTrueRegistration();
		client->setNick(nickname_sent);
	}
}
