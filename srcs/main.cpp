/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouchal <nsouchal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 10:10:09 by tnicolau          #+#    #+#             */
/*   Updated: 2024/10/21 13:01:34 by nsouchal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <cstdlib>

int	checkInput(char* port)
{
	char *end;
	long	res = strtol(port, &end, 10);

	if (res < 1024 || res > 65535 || *end != '\0')
		throw std::invalid_argument("Invalid port");
	return res;
}

int	main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Wrong number of arguments" << std::endl;
		return -1;
	}
	try
	{
		int port = checkInput(av[1]);
		Server	server(port, av[2]);

		struct sigaction	sa;
		sa.sa_flags = SA_SIGINFO;
		sa.sa_handler = Server::SignalHandler;
		sigaction(SIGINT, &sa, NULL);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return 0;
}
