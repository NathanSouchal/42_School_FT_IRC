/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 12:56:31 by tnicolau          #+#    #+#             */
/*   Updated: 2024/10/15 11:58:14 by tnicolau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
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
		int	serverSocketFd;
		std::vector<Client>	clients;
		std::vector<pollfd>	fds;
		static bool	_signal;
	public:
		Server();
		void	ServerSocket();
		void	AcceptNewClient();
		void	ServerProgram();
		void	CloseFds();
		void	ReceiveData(int fd);
		static void	SignalHandler(int signal);
		void		sendPing(int fd);
};

#endif
