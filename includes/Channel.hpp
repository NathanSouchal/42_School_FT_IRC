/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouchal <nsouchal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 12:34:13 by nsouchal          #+#    #+#             */
/*   Updated: 2024/10/21 16:09:50 by nsouchal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <vector>

class Client;

class Channel
{
    private:
    std::string           _name;
    std::string           _key;
    std::vector<Client *> channelClients;
    std::vector<Client *> channelOperators;
    bool                  _inviteOnly;
    public:
    Channel(const std::string &name);
    void  addChannelClient(Client *client);
    void  addChannelOperator(Client *client);
    const std::string   &getName();
    const std::string   &getKey();
    const bool          getInviteOnly();
    void                setKey(const std::string &key);
    void                setInviteOnly();
};

#endif