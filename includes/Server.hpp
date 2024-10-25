#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
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

class	Client;

class	Channel;

class	Server
{
	private:
		Server();
		int	_port;
		std::string	_password;
		int	serverSocketFd;
		int	_nbMaxClients;
		int	_nbUsers;
		std::vector<Client *>	clients;
		std::vector<pollfd>	fds;
		static bool	_signal;
		std::string	_creationTime;
		std::vector<Channel *> serverChannels;
	public:
		Server(int port, std::string password);
		~Server();
		void		setNbMaxClients(int newNb);
		const int	&getNbMaxClients();
		void		modifyNbUsers(int valueToAdd);
		const int	&getNbUsers();
		std::string	getCreationTime();
		void	ServerSocket();
		void	AcceptNewClient();
		void	clearClient(int fd);
		void	clearAllClients();
		void	ServerProgram();
		void	CloseFds();
		void	ReceiveData(int fd);
		static void	SignalHandler(int signal);
		void	SendPing(int fd);
		void	parseMessage(const std::string& message, int fd);
		void	checkCommand(const std::string& message, Client *current_client);

		void	password(const std::string& message, Client *client);
		void	nickname(const std::string& message, Client *client);
		void	user(const std::string& message, Client *client);
		void	motd(const std::string& message, Client *client);
		void	join(const std::string& message, Client *client);
		bool	checkAddClientToChannel(const std::string &name, const std::string &key, Client *client);
		void	createChannel(const std::string &name, const std::string &key, Client *client);
		void	privmsg(const std::string& message, Client *client);
		void	kick(const std::string& message, Client *client);
		void	invite(const std::string& message, Client *client);
		void	topic(const std::string& message, Client *client);
		void	lusers(const std::string& message, Client *client);

		Client	*findClient(int fd);
		bool	findNickName(const std::string& nick);

		void	addChannel(Channel *channel);
		Channel	*findChannel(const std::string& name);

		//MODE
		void	mode(const std::string& message, Client *client);
		std::vector<std::string>	parseModes(const std::string& src);
		bool	checkModeParams(const std::vector<std::string>& modes, const std::vector<std::string>& params);
};

#endif
