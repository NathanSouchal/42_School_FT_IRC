#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "utils.hpp"
#include "numerics.hpp"

void	Server::parseMessage(const std::string& message, int fd)
{
	std::string			command;
	std::istringstream	reader(message);
	std::string 		line, value;
	Client				*current_client = findClient(fd);

	if (message[message.size() - 1] != '\n')
		_partialCommand = _partialCommand + message;
	else
	{
		while (std::getline(reader, line))
		{
			if (!line.empty() && line[line.size() - 1] == '\r')
				line.erase(line.size() - 1);
			line = _partialCommand + line;
			_partialCommand.clear();
			command = line.substr(0, line.find(32));
			if (command != "PASS" && command != "CAP")
			{
				if (current_client->getPassword().empty())
				current_client->reply(ERR_NEEDMOREPARAMS(current_client->getNickname(), command));
			else
			{
				if (!current_client->getRegistration() && command != "NICK" && command != "USER")
					current_client->reply(ERR_NOTREGISTERED(current_client->getNickname(), command));
				else
					checkCommand(line, current_client);
			}
			}
			else if (command == "PASS")
				password(line, current_client);
		}
	}
}

void	Server::checkCommand(const std::string& message, Client *current_client)
{
	size_t 		pos = message.find(" ");
	std::string	command;

	if (pos == std::string::npos)
		command = message;
	else
		command = message.substr(0, pos);
	void(Server::*function_ptr[])(const std::string&, Client *) = {&Server::nickname, &Server::user,\
	&Server::lusers, &Server::join, &Server::privmsg, &Server::kick, &Server::invite, &Server::topic, &Server::mode, &Server::part, &Server::quiz};
	std::string commands[] = {"NICK", "USER", "LUSERS", "JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE", "PART", "QUIZ"};
	bool	found = false;

	for (size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
	{
		if (commands[i] == command)
		{
			if (pos == std::string::npos)
				current_client->reply(ERR_NEEDMOREPARAMS(current_client->getNickname(), command));
			else
				(this->*function_ptr[i])(message, current_client);
			found = true;
			break ;
		}
	}
	if (!found)
		current_client->reply(ERR_UNKNOWNCOMMAND(current_client->getNickname(), command));
	return ;
}
