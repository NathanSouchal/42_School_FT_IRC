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
		if (findNickName(nickname_sent))
		{
			client->reply(ERR_NICKNAMEINUSE(nickname_sent));
			return ;
		}
		client->setNick(nickname_sent);
		if (!(client->getRealname().empty()) && !(client->getUsername().empty()))
			client->setTrueRegistration();
	}
}
