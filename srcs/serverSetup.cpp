#include "Server.hpp"
#include "Client.hpp"
#include "numerics.hpp"

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
