#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "numerics.hpp"

void	Server::ServerSocket()
{
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

void	Server::clearClient(int fd)
{
	for (size_t i = 0; i < fds.size(); ++i)
	{
		if (fds[i].fd == fd)
		{
			fds.erase(fds.begin() + i);
			break ;
		}
	}
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i]->getFd() == fd)
		{
			if (clients[i]->getRegistration())
				modifyNbUsers(-1);
			delete clients[i];
			clients.erase(clients.begin() + i);
			break ;
		}
	}
}

void	Server::deleteAll()
{
	for (size_t i = 0; i < clients.size(); ++i)
		delete clients[i];
	clients.clear();
	for (size_t i = 0; i < serverChannels.size(); ++i)
		delete serverChannels[i];
	serverChannels.clear();
}

void	Server::AcceptNewClient()
{
	Client	*client = new Client(*this);
	sockaddr_in	clientAddress;
	pollfd	newPoll;
	socklen_t	len = sizeof(clientAddress);

	int	fd = accept(serverSocketFd, (sockaddr *)&clientAddress, &len);
	if (fd == -1)
	{
		delete client;
		std::cerr << "Accept() failed" << std::endl;
		return ;
	}
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		delete client;
		std::cerr << "fcntl() failed" << std::endl;
		return ;
	}
	try
	{
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
	}
	catch(...)
	{
		delete client;
		std::cerr << "Couldn't accept new client" << std::endl;
	}
}

void	Server::configBot()
{
	Client	*client = new Client(*this);
	sockaddr_in	clientAddress;
	pollfd	newPoll;
	socklen_t	len = sizeof(clientAddress);

	int	fd = accept(serverSocketFd, (sockaddr *)&clientAddress, &len);
	if (fd == -1)
	{
		delete client;
		throw std::runtime_error("Accept() failed");
	}
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		delete client;
		throw std::runtime_error("fcntl() failed");
	}

	newPoll.fd = fd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;

	client->setFd(fd);
	client->setIPaddress(inet_ntoa((clientAddress.sin_addr)));
	client->setNick("JulienLepers");
	client->setUsername("Jul");
	client->setRealname("Micheal Keaton");
	client->setTrueRegistration();
	client->setPassword("password");
	clients.push_back(client);
	fds.push_back(newPoll);
	if (fds.size() > static_cast<size_t>(_nbMaxClients))
		setNbMaxClients(clients.size());
	std::cout << "Client " << fd << " connected !" << std::endl;
}

void	Server::ReceiveData(int fd)
{
	char	buffer[1024];

	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1 , 0);

	if (bytes <= 0)
	{
		std::cerr << "Client " << fd << " disconnected" << std::endl;
		clearClient(fd);
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
