/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 10:10:09 by tnicolau          #+#    #+#             */
/*   Updated: 2024/10/14 13:57:00 by tnicolau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	main()
{
	Server	server;
	struct sigaction	sa;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_handler = Server::SignalHandler;

	try
	{
		sigaction(SIGINT, &sa, NULL);
		server.ServerProgram();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return 0;
}
