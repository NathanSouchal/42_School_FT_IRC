#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "utils.hpp"
#include "numerics.hpp"

void	Server::parseMessage(const std::string& message, int fd)
{
	std::cout << "Client " << fd << ", data: " << message << std::endl;
	std::string			command;
	std::istringstream	reader(message);
	std::string 		line, value;
	Client				*current_client = findClient(fd);

	while (std::getline(reader, line))
	{
		// std::cout << "line " << line << std::endl;
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		std::cout << "line " << line << std::endl;
		command = line.substr(0, line.find(32));
		std::cout << "commande " << command << "|"<< std::endl;
		if (command != "PASS" && command != "CAP")
		{
			if (current_client->getPassword().empty())
				current_client->reply(ERR_NEEDMOREPARAMS(current_client->getNickname(), command));
			else
				checkCommand(line, current_client);
		}
		else if (command == "PASS")
			password(line, current_client);
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
	void(Server::*function_ptr[])(const std::string&, Client *) = {&Server::nickname, &Server::user, &Server::motd,\
	&Server::lusers, &Server::join, &Server::privmsg, &Server::kick, &Server::invite, &Server::topic, &Server::mode};
	std::string commands[] = {"NICK", "USER", "MOTD", "LUSERS", "JOIN", "PRIVMSG", "KICK", "INVITE", "TOPIC", "MODE"};
	bool	found = false;

	for (size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
	{
		if (commands[i] == command)
		{
			(this->*function_ptr[i])(message, current_client);
			found = true;
			break ;
		}
	}
	if (!found)
		std::cerr << "Command " << message << " does not exist, sorry" << std::endl;
	return ;
}
