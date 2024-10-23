#include "Server.hpp"
#include "Client.hpp"
#include "numerics.hpp"

void	Server::motd(const std::string& message, Client *client)
{
	(void)message;
	client->reply(RPL_MOTDSTART(client->getNickname()));
	client->reply(RPL_MOTD(client->getNickname(), "Coucouuuuuu c'est le message du jooouuuur"));
	client->reply(RPL_ENDOFMOTD(client->getNickname()));
}
