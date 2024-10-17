/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouchal <nsouchal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 12:56:31 by tnicolau          #+#    #+#             */
/*   Updated: 2024/10/17 16:31:22 by nsouchal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <csignal>
#include <vector>
#include "Client.hpp"

class	Server
{
	private:
		Server();
		int	_port;
		std::string	_password;
		int	serverSocketFd;
		std::vector<Client *>	clients;
		std::vector<pollfd>	fds;
		static bool	_signal;
	public:
		Server(int port, std::string password);
		~Server();
		void	ServerSocket();
		void	AcceptNewClient();
		void	ServerProgram();
		void	CloseFds();
		void	ReceiveData(int fd);
		static void	SignalHandler(int signal);
		void	SendPing(int fd);
		void	parseMessage(const std::string& message, int fd);
		void	checkCommand(const std::string& message, Client *current_client);

		void	capabilityNegociation(const std::string& message);
		void	password(const std::string& message, Client *client);
		void	nickname(const std::string& message, Client *client);
		void	user(const std::string& message, Client *client);
		
		Client	*findClient(int fd);
};

#endif
