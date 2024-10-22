/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:34:13 by nsouchal          #+#    #+#             */
/*   Updated: 2024/10/22 15:07:05 by tnicolau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

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
	public:
		Channel(const std::string &name);
		void				addChannelClient(Client *client, const std::string& name);
		void				addChannelOperator(Client *client);

		Client*				getChannelOperator(const std::string& nickname);
		const std::string	&getName();
		const std::string	&getKey();
		bool				getInviteOnly();
		std::string			getChannelTopic();
		std::string			getTopicCreator();
		std::string			getTopicCreationTime();

		void				setKey(const std::string &key);
		void				setInviteOnly();
		void				setChannelTopic(const std::string& topic);
		void				setTopicCreator(const std::string& creator);
		void				setTopicCreationTime(const std::string& creationTime);
};

#endif
