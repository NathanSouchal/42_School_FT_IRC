#include "Client.hpp"
#include "Server.hpp"
#include "numerics.hpp"

Client::Client(Server& server) : _serverRef(server)
{
	_registrationChecked = false;
	_fd = -1;
	_nbQuestions = 0;
	_quizPoints = 0;
}

const int		&Client::getFd()
{
	return _fd;
}

const int		&Client::getNbQuestions()
{
	return _nbQuestions;
}

const std::string	&Client::getIPaddress()
{
	return _IPaddress;
}

const std::string	Client::getNickname()
{
	if (_nickname.empty())
		return "*";
	return _nickname;
}

bool	Client::checkEmptyNickname()
{
	if (_nickname.empty())
		return true;
	return false;
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

const std::vector<std::string>		&Client::getLastQuestion()
{
	return _lastQuestion;
}

std::vector<int>		&Client::getUsedIndex()
{
	return _usedIndex;
}

void	Client::setFd(const int& new_fd)
{
	_fd = new_fd;
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

void	Client::setLastQuestion(const std::vector<std::string>	&lastQuestion)
{
	_lastQuestion = lastQuestion;
}

void	Client::setNbQuestions(const int &nbQuestions)
{
	_nbQuestions = nbQuestions;
}

void	Client::setRealname(const std::string &realname)
{
	_realname = realname;
}

void	Client::reply(std::string message)
{
	send(_fd, message.c_str(), message.size(), 0);
}

void	Client::setTrueRegistration()
{
	_registrationChecked = true;
	std::cout << _nickname <<  " Registered!!" << std::endl;
	_serverRef.modifyNbUsers(1);
	reply(RPL_WELCOME(_nickname, _username, _IPaddress));
	reply(RPL_YOURHOST(_nickname));
	reply(RPL_CREATED(_nickname, _serverRef.getCreationTime()));
	reply(RPL_MYINFO(_nickname, "o", "klito kl"));
	reply(RPL_ISUPPORT(_nickname, "PREFIX=(o)@ CHANTYPES=#& CHANMODES=,k,l,it"));
}

bool	Client::checkRegistration()
{
	if (_username.empty() || _nickname.empty() || _password.empty() || _realname.empty())
		return false;
	return true;
}

void	Client::setQuizPoints(int points)
{
	_quizPoints = points;
}

int		Client::getQuizPoints()
{
	return _quizPoints;
}
