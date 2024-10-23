/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 12:56:29 by tnicolau          #+#    #+#             */
/*   Updated: 2024/10/23 11:42:59 by tnicolau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "numerics.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "utils.hpp"

bool	Server::_signal = false;

Server::Server(int port, std::string password) : _port(port), _password(password), serverSocketFd(0), _nbMaxClients(0), _nbUsers(0)
{
	_creationTime = getTimestamp();
	std::cout << "Password : " << _password << "\nPort : " << _port << std::endl;
	ServerSocket();
	ServerProgram();
}

Server::~Server()
{
	for (size_t i = 0; i < clients.size(); ++i)
		delete clients[i];
}

void		Server::setNbMaxClients(int newNb)
{
	_nbMaxClients = newNb;
}

const int	&Server::getNbMaxClients()
{
	return (_nbMaxClients);
}

void		Server::modifyNbUsers(int valueToAdd)
{
	_nbUsers += valueToAdd;
}

const int	&Server::getNbUsers()
{
	return (_nbUsers);
}

std::string	Server::getCreationTime()
{
	return _creationTime;
}

void	Server::SignalHandler(int signal)
{
	(void)signal;
	_signal = true;
}

void	Server::CloseFds()
{
	for (size_t i = 0; i < clients.size(); ++i)
	{
		std::cout << "Client " << clients[i]->getFd() << " disconnected" << std::endl;
		close(clients[i]->getFd());
	}
	close(serverSocketFd);
}

void	Server::ServerSocket()
{
	sockaddr_in serverAddress;
	pollfd	newPoll;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(_port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	int	optionval = 1;

	serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocketFd == -1)
		throw (std::runtime_error("Failed to create socket"));

	if (setsockopt(serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &optionval, sizeof(optionval)) == -1)
		throw(std::runtime_error("Failed to set option (SO_REUSEADDR) on socket"));

	if (fcntl(serverSocketFd, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("Failed to set option (O_NONBLOCK) on socket"));

	if (bind(serverSocketFd, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
		throw (std::runtime_error("Failed to bind socket"));

	if (listen(serverSocketFd, SOMAXCONN) == -1)
		throw(std::runtime_error("Failed to listen"));

	newPoll.fd = serverSocketFd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	fds.push_back(newPoll);
}

void	Server::AcceptNewClient()
{
	Client	*client = new Client(*this);
	sockaddr_in	clientAddress;
	pollfd	newPoll;
	socklen_t	len = sizeof(clientAddress);

	int	fd = accept(serverSocketFd, (sockaddr *)&clientAddress, &len);
	if (fd == -1)
		std::cerr << "Accept() failed" << std::endl;
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "fcntl() failed" << std::endl;
		return ;
	}

	newPoll.fd = fd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;

	client->setFd(fd);
	client->setIPaddress(inet_ntoa((clientAddress.sin_addr)));
	clients.push_back(client);
	fds.push_back(newPoll);
	if (fds.size() > static_cast<size_t>(_nbMaxClients))
		setNbMaxClients(clients.size());
	std::cout << "Client " << fd << " connected !" << std::endl;
	//sendPing(fd);
}

void	Server::SendPing(int fd)
{
	std::string ping = "PING :serverping\r\n";
	send(fd, ping.c_str(), ping.length(), 0);
	time_t timePing = time(NULL);
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i]->getFd() == fd)
			clients[i]->setPing(timePing);
	}
}

void	Server::ReceiveData(int fd)
{
	char	buffer[1024];

	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1 , 0); //-> receive the data

	if (bytes == 0)
	{ //-> check if the client disconnected
		std::cout << "Client " << fd << " disconnected" << std::endl;
		//ClearClients(fd); //-> clear the client
		close(fd); //-> close the client socket
	}
	else if (bytes < 0)
	{
		std::cout << "Client " << fd << " error made him disconnect" << std::endl;
		close(fd);
	}
	else
	{
		buffer[bytes] = '\0';
		parseMessage(buffer, fd);
	}
}

void	Server::password(const std::string& message, Client *client)
{
	int			pos;
	std::string	password_sent;

	pos = message.find(32);
	password_sent = message.substr(pos + 1);
	if (client->checkRegistration())
		client->reply(ERR_ALREADYREGISTERED(client->getNickname(), "PASS"));
	else if (password_sent == _password)
		client->setPassword(password_sent);
	else
		client->reply(ERR_PASSWDMISMATCH(client->getNickname(), "PASS"));
}

Client	*Server::findClient(int fd)
{
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i]->getFd() == fd)
			return clients[i];
	}
	return NULL;
}

bool	Server::findNickName(const std::string& nick)
{
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i]->getNickname() == nick)
			return true;
	}
	return false;
}

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

void	Server::ServerProgram()
{
	while (!_signal)
	{
		//Attend qu'il se produise un evenement sur un des fds
		if((poll(&fds[0],fds.size(), -1) == -1) && !_signal)
			throw(std::runtime_error("poll() failed"));
		for (size_t i = 0; i < fds.size(); i++)
		{
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == serverSocketFd)
					AcceptNewClient();
				else
					ReceiveData(fds[i].fd);
			}
		}
	}
	CloseFds();
}

void	Server::addChannel(Channel *channel)
{
	serverChannels.push_back(channel);
}

Channel	*Server::findChannel(const std::string& channel)
{
	for (std::vector<Channel*>::iterator it = serverChannels.begin(); it != serverChannels.end(); ++it)
	{
		if ((*it)->getName() == channel)
			return *it;
	}
	return NULL;
}
