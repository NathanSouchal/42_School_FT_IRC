#include "Client.hpp"
#include "Server.hpp"
#include "numerics.hpp"

Client::Client(Server& server) : _serverRef(server)
{
	_nickname = "*";
}

const int		&Client::getFd()
{
	return _fd;
}

const std::string	&Client::getIPaddress()
{
	return _IPaddress;
}

const std::string	&Client::getNickname()
{
	return _nickname;
}

const std::string	&Client::getUsername()
{
	return _username;
}


const std::string	&Client::getRealname()
{
	return _realname;
}

const std::string	&Client::getPassword()
{
	return _password;
}

const bool	&Client::getRegistration()
{
	return _registrationChecked;
}

Server		&Client::getServerRef()
{
	return _serverRef;
}

void	Client::setFd(const int& new_fd)
{
	_fd = new_fd;
}

void	Client::setPing(time_t timePing)
{
	_timePing = timePing;
}

void	Client::setPong(time_t timePong)
{
	_timePong = timePong;
}

void	Client::setIPaddress(const std::string& new_ip)
{
	_IPaddress = new_ip;
}

void	Client::setNick(const std::string &nickname)
{
	_nickname = nickname;
}

void	Client::setPassword(const std::string &password)
{
	_password = password;
}

void	Client::setUsername(const std::string &username)
{
	_username = username;
}

void	Client::setRealname(const std::string &realname)
{
	_realname = realname;
}

void	Client::reply(std::string message)
{
	send(_fd, message.c_str(), message.size(), 0);
	std::cout << "fd: " << _fd << ", message: " << message << std::endl;
}

void	Client::setTrueRegistration()
{
	_registrationChecked = true;
	std::cout << "Registered!!" << std::endl;
	_serverRef.modifyNbUsers(1);
	reply(RPL_WELCOME(_nickname, _username, _IPaddress));
	reply(RPL_YOURHOST(_nickname));
	reply(RPL_CREATED(_nickname, _serverRef.getCreationTime()));
	reply(RPL_MYINFO(_nickname, "o", "t"));
	reply(RPL_ISUPPORT(_nickname, "PREFIX=(ov)@+ CHANTYPES=#& CHANMODES=b,k,l,imnt"));
}

bool	Client::checkRegistration()
{
	if (_username.empty() || _nickname.empty() || _password.empty() || _realname.empty())
		return false;
	return true;
}
