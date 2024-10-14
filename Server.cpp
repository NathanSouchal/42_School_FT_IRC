/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 12:56:29 by tnicolau          #+#    #+#             */
/*   Updated: 2024/10/14 10:44:26 by tnicolau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::ServerSocket()
{
	sockaddr_in serverAddress;
	pollfd	newPoll;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	std::cout << INADDR_ANY << std::endl;

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

void	Server::ServerProgram()
{
	ServerSocket();
	while (42)
	{
		if((poll(&fds[0],fds.size(),-1) == -1))
			throw(std::runtime_error("poll() failed"));
		for (size_t i = 0; i < fds.size(); i++)
		{
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == serverSocketFd)
					AcceptNewClient();
			}
		}
	}

}
