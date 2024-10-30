#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <string>

class Client;

class Channel
{
	private:
		std::string				_name;
		std::string				_key;
		std::vector<Client *>	channelClients;
		std::vector<Client *>	channelOperators;
		std::string				_channelTopic;
		std::string				_topicCreator;
		std::string				_topicCreationTime;
		bool					_inviteOnly;
		bool					_modeT;
		int						_userLimit;
	public:
		Channel(const std::string &name);
		void					addChannelClient(Client *client);
		void					addChannelOperator(Client *client);
		void					deleteChannelClient(Client *client);
		void					deleteChannelOperator(Client *client);
		void					removeClientFromChannel(Client *client);
		std::string				generateUserList();
		void					replySuccessfullJoin(Client *client);

		Client*					findOperatorInChannel(const std::string& nickname);
		Client*					findClientInChannel(const std::string& nickname);
		const std::string		&getName();
		const std::string		&getKey();
		bool					getInviteOnly();
		std::string				getChannelTopic();
		std::string				getTopicCreator();
		std::string				getTopicCreationTime();
		bool					getModeT();
		int						getUserLimit();
		std::string				getActiveModes();
		std::vector<Client *>	getClientList();

		void					setKey(const std::string &key);
		void					setInviteOnly();
		void					setChannelTopic(const std::string& topic);
		void					setTopicCreator(const std::string& creator);
		void					setTopicCreationTime(const std::string& creationTime);
		void					setModeT();
		void					sendMessageToAllClients(Client *client, const std::string& src, const std::string& param1, const std::string& param2);
		void					informUsersOnJoin(Client *client);
		void					setUserLimit(int limit);
};

#endif
