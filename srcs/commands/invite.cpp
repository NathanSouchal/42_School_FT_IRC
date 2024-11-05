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
		return client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "INVITE"));
	userInvited = parameters[0];
	channelName = parameters[1];
	if (!findClientByNickname(userInvited))
		return client->reply(ERR_NOSUCHNICK(client->getNickname(), userInvited));
	if (!findChannel(channelName))
		return client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
	if (!(findChannel(channelName)->findClientInChannel(client->getNickname())))
		return client->reply(ERR_NOTONCHANNEL(client->getNickname(), channelName));
	if (findChannel(channelName)->findClientInChannel(userInvited))
		return client->reply(ERR_USERONCHANNEL(client->getNickname(), userInvited, channelName));;
	if (!(findChannel(channelName)->checkIfUserOperator(client->getNickname())) &&  \
	findChannel(channelName)->getInviteOnly())
		return client->reply(ERR_CHANOPRIVSNEEDED(client->getNickname(), channelName));
	findClientByNickname(userInvited)->reply(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getIPaddress() \
	+ " INVITE " + userInvited + " :" + channelName + "\r\n");
	client->reply(RPL_INVITING(client->getNickname(), userInvited, channelName));
}
