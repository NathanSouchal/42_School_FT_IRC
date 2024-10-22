/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:34:13 by nsouchal          #+#    #+#             */
/*   Updated: 2024/10/22 09:30:11 by tnicolau         ###   ########.fr       */
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
		std::vector<std::string>	channelTopic;
		bool					_inviteOnly;
	public:
		Channel(const std::string &name);
		void				addChannelClient(Client *client, const std::string& name);
		void				addChannelOperator(Client *client);
		const std::string	&getName();
		const std::string	&getKey();
		bool				getInviteOnly();
		void				setKey(const std::string &key);
		void				setInviteOnly();
};

#endif
