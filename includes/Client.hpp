#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <ctime>
#include <sys/socket.h>
#include <sys/types.h>

class	Server;

class	Client
{
	private:
		int			_fd;
		std::string	_IPaddress;
		std::string	_nickname;
		std::string	_username;
		std::string	_realname;
		std::string	_password;
		bool		_registrationChecked;
		Server&		_serverRef;
	public:
		Client(Server& server);
		const int			&getFd();
		const std::string	&getIPaddress();
		const std::string	getNickname();
		const std::string	&getUsername();
		const std::string	&getRealname();
		const std::string	&getPassword();
		Server&				getServerRef();
		const bool			&getRegistration();
		void				setFd(const int& new_fd);
		void				setNick(const std::string &nickname);
		void				setUsername(const std::string &username);
		void				setRealname(const std::string &realname);
		void				setPassword(const std::string &password);
		void				setTrueRegistration();
		void				setIPaddress(const std::string& new_ip);
		bool				checkRegistration();
		bool				checkEmptyNickname();
		void				reply(std::string message);
};

#endif
