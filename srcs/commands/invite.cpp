#include "Server.hpp"
#include "Client.hpp"
#include "numerics.hpp"
#include "utils.hpp"
#include "Channel.hpp"

void	Server::invite(const std::string& message, Client *client)
{
	std::vector<std::string>	parameters = parseParams(message.substr(message.find(" ")));
	std::string					userInvited;
	std::string					channelName;

	if (parameters.size() != 2)
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "INVITE"));
		return ;
	}
	userInvited = parameters[0];
	channelName = parameters[1];
	if (!findClientByNickname(userInvited))
	{
		client->reply(ERR_NOSUCHNICK(client->getNickname(), userInvited));
		return;
	}
	if (!findChannel(channelName))
	{
		client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
		return;
	}
	if (!(findChannel(channelName)->findClientInChannel(client->getNickname())))
	{
		client->reply(ERR_NOTONCHANNEL(client->getNickname(), channelName));
		return;
	}
	if (findChannel(channelName)->findClientInChannel(userInvited))
	{
		client->reply(ERR_USERONCHANNEL(client->getNickname(), userInvited, channelName));
		return;
	}
	if (!(findChannel(channelName)->findOperatorInChannel(client->getNickname())) &&  \
	findChannel(channelName)->getInviteOnly())
	{
		client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channelName));
		return;
	}

	findClientByNickname(userInvited)->reply(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getIPaddress() \
	+ " INVITE " + userInvited + " :" + channelName + "\r\n");
	client->reply(RPL_INVITING(client->getNickname(), userInvited, channelName));
}
