/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 12:56:29 by tnicolau          #+#    #+#             */
/*   Updated: 2024/10/15 12:05:06 by tnicolau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool	Server::_signal = false;

Server::Server() : serverSocketFd(0)
{}

void	Server::SignalHandler(int signal)
{
	(void)signal;
	_signal = true;
}

void	Server::CloseFds()
{
	for (size_t i = 0; i < clients.size(); ++i)
	{
		std::cout << "Client " << clients[i].getFd() << " disconnected" << std::endl;
		close(clients[i].getFd());
	}
	close(serverSocketFd);
}

void	Server::ServerSocket()
{
	sockaddr_in serverAddress;
	pollfd	newPoll;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
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
	Client	client;
	sockaddr_in	clientAddress;
	pollfd	newPoll;
	socklen_t	len = sizeof(clientAddress);

	int	fd = accept(serverSocketFd, (sockaddr *)&clientAddress, &len);
	if (fd == -1)
		std::cerr << "Accept() failed" << std::endl;
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cout << "fcntl() failed" << std::endl;
		return ;
	}

	newPoll.fd = fd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;

	client.setFd(fd);
	client.setIPaddress(inet_ntoa((clientAddress.sin_addr)));
	clients.push_back(client);
	fds.push_back(newPoll);

	std::cout << "Client " << fd << " connected !" << std::endl;
}

void	Server::sendPing(int fd)
{
	std::string ping = "PING :serverping\r\n";
	send(fd, ping.c_str(), ping.length(), 0);
	time_t timePing = time(NULL);
	for (int i = 0; i < clients.size(); ++i)
	{
		if (clients[i].getFd() == fd)
			clients[i].setPing(timePing);
	}
}

void	Server::ReceiveData(int fd)
{
	char	buffer[1024];

	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1 , 0); //-> receive the data

	if(bytes <= 0)
	{ //-> check if the client disconnected
		std::cout << "Client " << fd << " Disconnected" << std::endl;
		//ClearClients(fd); //-> clear the client
		close(fd); //-> close the client socket
	}

	else
	{ //-> print the received data
		buffer[bytes] = '\0';
		std::cout << "Client " << fd << " Data: " << buffer;
		//here you can add your code to process the received data: parse, check, authenticate, handle the command, etc...
	}
}

void	Server::ServerProgram()
{
	ServerSocket();
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
