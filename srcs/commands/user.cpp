#include "Server.hpp"
#include "Client.hpp"
#include "numerics.hpp"

void	Server::user(const std::string& message, Client *client)
{
	size_t		pos = message.find(32);
	std::string	username_sent;
	std::string	realname_sent;

	if (pos == std::string::npos)
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "USER"));
	else
	{
		size_t		pos2 =(message.substr(pos + 1)).find(32);

		if (pos2 == std::string::npos)
			client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "USER"));
		else
		{
			username_sent = message.substr(pos + 1, pos2);
			client->setUsername(username_sent);
			pos = message.find(":");
			if (pos == std::string::npos)
				client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "USER"));
			else
			{
				realname_sent = message.substr(pos + 1);
				client->setRealname(realname_sent);
				if (client->getNickname() != "*")
					client->setTrueRegistration();
			}
		}
	}
}
