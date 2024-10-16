/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 12:56:31 by tnicolau          #+#    #+#             */
/*   Updated: 2024/10/16 13:04:40 by tnicolau         ###   ########.fr       */
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
		std::vector<Client>	clients;
		std::vector<pollfd>	fds;
		static bool	_signal;
	public:
		Server(int port, std::string password);
		void	ServerSocket();
		void	AcceptNewClient();
		void	ServerProgram();
		void	CloseFds();
		void	ReceiveData(int fd);
		static void	SignalHandler(int signal);
		void	SendPing(int fd);
		void	parseMessage(char* message, int fd);
		void	checkCommand(const std::string& message);

		void	capabilityNegociation(const std::string& message);
		void	password(const std::string& message);
		void	nickname(const std::string& message);
		void	user(const std::string& message);
};

#endif
