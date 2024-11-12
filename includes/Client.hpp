#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <ctime>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>

class	Server;

class	Client
{
	private:
		int							_fd;
		std::string					_IPaddress;
		std::string					_nickname;
		std::string					_username;
		std::string					_realname;
		std::string					_password;
		bool						_registrationChecked;
		Server&						_serverRef;
		std::vector<std::string>	_lastQuestion;
		int							_nbQuestions;
		std::vector<int>			_usedIndex;
		int							_quizPoints;
	public:
		Client(Server& server);
		const int			&getFd();
		const int			&getNbQuestions();
		const std::string	&getIPaddress();
		const std::string	getNickname();
		const std::string	&getUsername();
		const std::string	&getRealname();
		const std::string	&getPassword();
		Server&				getServerRef();
		const bool			&getRegistration();
		const std::vector<std::string>	&getLastQuestion();
		std::vector<int>	&getUsedIndex();
		void				setQuizPoints(int points);
		int					getQuizPoints();
		void				setFd(const int& new_fd);
		void				setNick(const std::string &nickname);
		void				setUsername(const std::string &username);
		void				setRealname(const std::string &realname);
		void				setPassword(const std::string &password);
		void				setTrueRegistration();
		void				setLastQuestion(const std::vector<std::string>	&lastQuestion);
		void				setNbQuestions(const int &nbQuestions);
		void				setIPaddress(const std::string& new_ip);
		bool				checkRegistration();
		bool				checkEmptyNickname();
		void				reply(std::string message);
};

#endif
