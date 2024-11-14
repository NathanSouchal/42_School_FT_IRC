#include "Server.hpp"
#include "Client.hpp"
#include "numerics.hpp"
#include "utils.hpp"
#include "Channel.hpp"

void	Server::privmsg(const std::string& message, Client *client)
{
	std::vector<std::string>	parameters = parseParams(message.substr(message.find(" ")));
	std::vector<std::string>	targets;
	std::string					msgToSend, temp;

	if (parameters.size() != 2)
	{
		if (parameters.size() > 2)
			client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PRIVMSG"));
		else if ((parameters.size() == 1 && parameters[0][0] == ':') || parameters.size() == 0)
			client->reply(ERR_NORECIPIENT(client->getNickname(), "PRIVMSG"));
		else if (parameters.size() == 1)
			client->reply(ERR_NOTEXTTOSEND(client->getNickname()));
		return ;
	}
	msgToSend = parameters[1];
	if (msgToSend[0] == ':')
		msgToSend = msgToSend.substr(msgToSend.find(':') + 1);
	std::stringstream			ss(parameters[0]);
	while (std::getline(ss, temp, ','))
	{
		if (!temp.empty())
			targets.push_back(temp);
	}
	for (size_t i = 0; i < targets.size(); ++i)
	{
		if (targets[i][0] == '&' || targets[i][0] == '#')
			messageToChannel(msgToSend, client, targets[i]);
		else
			messageToUser(msgToSend, client, targets[i]);

	}
}

void	Server::messageToChannel(const std::string& msgToSend, Client *client, std::string channelName)
{
	std::string				commandToSend;
	Channel					*channel;
	std::vector<Client *>	channelClients;

	channel = findChannel(channelName);
	if (!channel)
		return client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channelName));
	if (!channel->findClientInChannel(client->getNickname()))
		return client->reply(ERR_NOTONCHANNEL(client->getNickname(), channelName));
	commandToSend = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getIPaddress() \
	+ " PRIVMSG " + channelName + " :" + msgToSend + "\r\n";
	channelClients = channel->getClientList();
	for (size_t i = 0; i < channelClients.size(); ++i)
	{
		if (client != channelClients[i])
			channelClients[i]->reply(commandToSend);
	}
}

void	Server::answerQuiz(const std::string& msgToSend, Client *client, Client *bot)
{
	std::string	commandToSend;

	if (!client->getLastQuestion().empty())
	{
		if (client->getLastQuestion()[4] == msgToSend)
		{
			privmsg(" " + client->getNickname() + " :Well done !", bot);
			client->setQuizPoints(client->getQuizPoints() + 1);
			privmsg(" " + client->getNickname() + " :+ 1 points (total " + convertInString(client->getQuizPoints()) + " points)", bot);
		}
		else
		{
			privmsg(" " + client->getNickname() + " :Bad answer, the right answer was : " + client->getLastQuestion()[4], bot);
			privmsg(" " + client->getNickname() + " :+ 0 points (total " + convertInString(client->getQuizPoints()) + " points)", bot);
		}
	}
	if (!client->getNbQuestions())
	{
		if (!client->getLastQuestion().empty())
		{
			client->setLastQuestion(std::vector<std::string>());
			privmsg(" " + client->getNickname() + " :Total points for this game : " + convertInString(client->getQuizPoints()) + " points", bot);
			client->setQuizPoints(0);
			client->getUsedIndex().clear();
		}
		return privmsg(" " + client->getNickname() + " :Game is over. To start a new game, send a new QUIZ command", bot);
	}
	sendQuestion(client, bot);
}

void	Server::messageToUser(const std::string& msgToSend, Client *client, std::string targetUserName)
{
	std::string	commandToSend;
	Client		*targetUser;

	targetUser = findClientByNickname(targetUserName);
	if (!targetUser)
		return client->reply(ERR_NOSUCHNICK(client->getNickname(), targetUserName));
	if (targetUserName == "JulienLepers")
		answerQuiz(msgToSend, client, targetUser);
	commandToSend = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getIPaddress() \
	+ " PRIVMSG " + targetUser->getNickname() + " :" + msgToSend + "\r\n";
	targetUser->reply(commandToSend);
}
