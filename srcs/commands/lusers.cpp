#include "Server.hpp"
#include "Client.hpp"
#include "numerics.hpp"
#include "utils.hpp"

void	Server::lusers(const std::string& message, Client *client)
{
	(void)message;

	client->reply(RPL_LUSERCLIENT(client->getNickname(), convertInString(clients.size()), "0"));
	client->reply(RPL_LUSEROP(client->getNickname(), "1"));
	client->reply(RPL_LUSERUNKNOWN(client->getNickname(), convertInString(clients.size() - _nbUsers)));
	client->reply(RPL_LUSERCHANNELS(client->getNickname(), convertInString(serverChannels.size())));
	client->reply(RPL_LUSERME(client->getNickname(), convertInString(clients.size())));
	client->reply(RPL_LOCALUSERS(client->getNickname(), convertInString(clients.size()), convertInString(_nbMaxClients)));
	client->reply(RPL_GLOBALUSERS(client->getNickname(), convertInString(clients.size()), convertInString(_nbMaxClients)));
}
