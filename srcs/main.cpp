#include "Server.hpp"

bool	_signal = false;

int	checkInput(char* port)
{
	char *end;
	long	res = strtol(port, &end, 10);

	if (res < 1024 || res > 65535 || *end != '\0')
		throw std::invalid_argument("Invalid port");
	return res;
}

void	SignalHandler(int signal)
{
	if (signal == SIGINT)
		_signal = true;
}

int	main(int ac, char **av)
{
	if (ac != 4)
	{
		std::cerr << "Wrong number of arguments" << std::endl;
		return -1;
	}
	try
	{
		struct sigaction	sa;
		std::memset(&sa, 0, sizeof(sa));
		sa.sa_flags = SA_SIGINFO;
		sa.sa_handler = SignalHandler;
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGTSTP, &sa, NULL);

		int port = checkInput(av[1]);
		Server	server(port, av[2]);
		server.connectBot();
		server.configBot();
		server.parseQuizDatabase(av[3]);
		while (!_signal)
		{
			server.ServerProgram(_signal);
			sleep(1);
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return 0;
}

