#include "Server.hpp"
#include "numerics.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "utils.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password), serverSocketFd(0), _nbMaxClients(0), _nbUsers(0)
{
	_creationTime = getTimestampDate();
	std::cout << "Password : " << _password << "\nPort : " << _port << std::endl;
	ServerSocket();
}

void	Server::ServerProgram(bool _signal)
{
	if (_signal)
		return;
	//Attend qu'il se produise un evenement sur un des fds
	if((poll(&fds[0],fds.size(), -1) == -1) && !_signal)
		return ;
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

void	Server::CloseFds()
{
	for (size_t i = 0; i < clients.size(); ++i)
	{
		std::cout << "Client " << clients[i]->getFd() << " disconnected" << std::endl;
		close(clients[i]->getFd());
	}
	close(serverSocketFd);
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

Client	*Server::findClient(int fd)
{
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i]->getFd() == fd)
			return clients[i];
	}
	return NULL;
}

Client	*Server::findClientByNickname(std::string nickname)
{
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i]->getNickname() == nickname)
			return clients[i];
	}
	return NULL;
}

bool	Server::findNickName(const std::string& nick)
{
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i]->checkRegistration() && clients[i]->getNickname() == nick)
			return true;
	}
	return false;
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
