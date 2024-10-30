#include "Channel.hpp"
#include "Client.hpp"
#include "numerics.hpp"

Channel::Channel(const std::string &name): _name(name)
{
	_userLimit = 10;
}

const std::string	&Channel::getName()
{
	return (_name);
}

const std::string	&Channel::getKey()
{
	return (_key);
}

bool	Channel::getInviteOnly()
{
	return _inviteOnly;
}

void	Channel::setInviteOnly()
{
	_inviteOnly = !_inviteOnly;
}

void	Channel::setKey(const std::string &key)
{
	_key = key;
}

std::string	Channel::generateUserList()
{
	std::string	userList;

	for (size_t i = 0; i < channelOperators.size(); ++i)
	{
		userList = userList + "@" + channelOperators[i]->getNickname();
		if (i != channelOperators.size() - 1)
			userList + " ";
	}
	for (size_t i = 0; i < channelClients.size(); ++i)
	{
		if (std::find(channelOperators.begin(), channelOperators.end(), channelClients[i]) ==  channelOperators.end())
			userList = userList + " " + channelClients[i]->getNickname();
	}
	return userList;
}

void	Channel::replySuccessfullJoin(Client *client)
{
	client->reply(JOIN(client->getNickname(), client->getUsername(), client->getIPaddress(), _name));
	if (!_channelTopic.empty())
	{
		client->reply(RPL_TOPIC(client->getNickname(), _name, _channelTopic));
		client->reply(RPL_TOPICWHOTIME(client->getNickname(), _name, _topicCreator, _topicCreationTime));
	}
	client->reply(RPL_NAMREPLY(client->getNickname(), _name, generateUserList()));
	client->reply(RPL_ENDOFNAMES(client->getNickname(), _name));
}

void	Channel::addChannelClient(Client *client)
{
	std::vector<Client*>::iterator it;

	if (channelClients.size() >= _userLimit)
	{
		client->reply(ERR_CHANNELISFULL(client->getNickname(), _name));
		return ;
	}
	it = std::find(channelClients.begin(), channelClients.end(), client);
	if (it != channelClients.end() || !client)
		return ;
	channelClients.push_back(client);
	replySuccessfullJoin(client);
}

void	Channel::addChannelOperator(Client *client)
{
	std::vector<Client*>::iterator it = std::find(channelOperators.begin(), channelOperators.end(), client);
	if (it != channelOperators.end() || !client)
		return ;
	channelOperators.push_back(client);
	std::cout << "Added channel Operator " << client->getNickname() << std::endl;
}

void	Channel::deleteChannelOperator(Client *client)
{
	std::vector<Client*>::iterator it = std::find(channelOperators.begin(), channelOperators.end(), client);
	if (it != channelOperators.end())
	{
		channelOperators.erase(it);
		std::cout << "Deleted channel Operator " << client->getNickname() << std::endl;
	}
}

void	Channel::removeClientFromChannel(Client *client)
{
	std::vector<Client*>::iterator it = std::find(channelClients.begin(), channelClients.end(), client);
	if (it != channelClients.end())
		channelClients.erase(it);
}


Client*		Channel::findOperatorInChannel(const std::string& nickname)
{
	for (std::vector<Client*>::iterator it = channelOperators.begin(); it != channelOperators.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
			return *it;
	}
	return NULL;
}

Client*		Channel::findClientInChannel(const std::string& nickname)
{
	for (std::vector<Client*>::iterator it = channelClients.begin(); it != channelClients.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
			return *it;
	}
	return NULL;
}

std::string		Channel::getChannelTopic()
{
	return this->_channelTopic;
}

std::string		Channel::getTopicCreator()
{
	return this->_topicCreator;
}

std::string		Channel::getTopicCreationTime()
{
	return this->_topicCreationTime;
}

bool		Channel::getModeT()
{
	return this->_modeT;
}

int			Channel::getUserLimit()
{
	return this->_userLimit;
}

std::string	Channel::getActiveModes()
{
	std::string	result = "+";

	if (getInviteOnly())
		result += "i";
	if (getModeT())
		result += "t";
	if (!getKey().empty())
		result += "k";
	if (channelOperators.size())
		result += "o";
	result += "l";
	return result;
}

void		Channel::setModeT()
{
	this->_modeT = !_modeT;
}

void		Channel::setChannelTopic(const std::string& topic)
{
	this->_channelTopic = topic;
}

void		Channel::setTopicCreator(const std::string& creator)
{
	this->_topicCreator = creator;
}

void		Channel::setTopicCreationTime(const std::string& creationTime)
{
	std::cout << "creationTime :" << creationTime << std::endl;
	this->_topicCreationTime = creationTime;
}

void		Channel::setUserLimit(int limit)
{
	this->_userLimit = limit;
}

void		Channel::sendMessageToAllClients(const std::string& src, const std::string& param1, const std::string& param2)
{
	for (std::vector<Client*>::iterator it = channelClients.begin(); it != channelClients.end(); ++it)
	{
		if (src == "TOPIC")
			(*it)->reply(TOPIC((*it)->getNickname(), (*it)->getUsername(), this->getName(), (*it)->getIPaddress(), this->getChannelTopic()));
		else if (src == "MODE")
			(*it)->reply(MODE((*it)->getNickname(), (*it)->getUsername(), (*it)->getIPaddress(), this->getName(), param1, param2));
	}
}

std::vector<Client *>	Channel::getClientList()
{
	return channelClients;
}

void	Channel::informUsersOnJoin(Client *client)
{
	for (size_t i = 0; i < channelClients.size(); ++i)
	{
		if (channelClients[i] != client)
		{
			channelClients[i]->reply(JOIN(client->getNickname(), client->getUsername(), client->getIPaddress(), _name));
			channelClients[i]->reply(RPL_NAMREPLY(client->getNickname(), _name, generateUserList()));
			channelClients[i]->reply(RPL_ENDOFNAMES(client->getNickname(), _name));
		}
	}
}