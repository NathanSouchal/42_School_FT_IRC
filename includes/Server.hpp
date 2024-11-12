#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <csignal>
#include <vector>
#include <map>
#include <cstring>
#include <cerrno>

class	Client;

class	Channel;

class	Server
{
	private:
		Server();
		int	_port;
		std::string	_password;
		sockaddr_in serverAddress;
		int	serverSocketFd;
		int	_nbMaxClients;
		int	_nbUsers;
		std::vector<Client *>	clients;
		std::vector<pollfd>	fds;
		std::string	_creationTime;
		std::vector<Channel *>	serverChannels;
		std::string				_partialCommand;
		std::vector<std::vector<std::string> > _quizDataBase;
	public:
		Server(int port, std::string password);
		~Server();
		void				setNbMaxClients(int newNb);
		const int			&getNbMaxClients();
		void				modifyNbUsers(int valueToAdd);
		const int			&getNbUsers();
		std::string	getCreationTime();
		void	ServerSocket();
		void	AcceptNewClient();
		void	configBot();
		void	clearClient(int fd);
		void	deleteAll();
		void	ServerProgram(bool _signal);
		void	CloseFds();
		void	ReceiveData(int fd);
		void	parseMessage(const std::string& message, int fd);
		void	checkCommand(const std::string& message, Client *current_client);
		void	deleteChannel(Channel *channel);

		void	password(const std::string& message, Client *client);
		void	nickname(const std::string& message, Client *client);
		void	user(const std::string& message, Client *client);
		void	part(const std::string& message, Client *client);

		void	join(const std::string& message, Client *client);
		bool	checkAddClientToChannel(const std::string &name, const std::string &key, Client *client);
		void	createChannel(const std::string &name, const std::string &key, Client *client);

		void	privmsg(const std::string& message, Client *client);
		void	messageToChannel(const std::string& msgToSend, Client *client, std::string channelName);
		void	messageToUser(const std::string& msgToSend, Client *client, std::string targetUserName);

		void	invite(const std::string& message, Client *client);
		void	topic(const std::string& message, Client *client);
		void	sendTopic(Channel *channel, Client *client);
		void	lusers(const std::string& message, Client *client);

		Client	*findClient(int fd);
		Client	*findClientByNickname(std::string nickname);
		bool	findNickName(const std::string& nick);

		void	addChannel(Channel *channel);
		Channel	*findChannel(const std::string& name);

		//MODE
		void	mode(const std::string& message, Client *client);
		std::vector<std::string>	parseModes(const std::string& src, Client *client);
		std::map<std::string, std::string>	joinModesAndParams(const std::vector<std::string>& modes, const std::vector<std::string>& params, Client *client);
		void	setNewModes(std::map<std::string, std::string> modesAndParams, Channel* channel, Client* client);

		//KICK
		void	kick(const std::string& message, Client *client);
		std::vector<std::string>	parseUsers(const std::string& src);

		//QUIZ
		void	connectBot();
		void	parseQuizDatabase(const std::string& database);
		void	quiz(const std::string& message, Client *client);
		void	answerQuiz(const std::string& msgToSend, Client *client, Client *bot);
		void	sendQuestion(Client *client, Client *bot);
};

#endif
