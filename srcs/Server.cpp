#include "Server.hpp"
#include "numerics.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "utils.hpp"

Server::Server(int port, std::string password) : _port(port), _password(password), serverSocketFd(0), _nbMaxClients(0), _nbUsers(0)
{
	_creationTime = getTimestampDate();
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
	CloseFds();
	deleteAll();
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

void	Server::deleteChannel(Channel *channel)
{
	if (!(channel->getClientList().size()) && !(channel->getOperatorList().size()))
	{
		if (findChannel(channel->getName()))
		{
			for (std::vector<Channel *>::iterator it = serverChannels.begin(); it != serverChannels.end(); ++it)
			{
				if (*it == channel)
				{
					serverChannels.erase(it);
					delete channel;
					break ;
				}
			}
		}
	}
}

void	Server::connectBot()
{
	int botSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (botSocket == -1)
	{
        throw std::runtime_error("socket() failed for bot");
	}
	if (connect(botSocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
		close(botSocket);
        throw std::runtime_error("connect() failed for bot");
    }
    std::cout << "Bot connected to the server successfully!" << std::endl;
}
