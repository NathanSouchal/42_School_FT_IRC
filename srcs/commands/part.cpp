#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "numerics.hpp"
#include "utils.hpp"

void	Server::part(const std::string& message, Client *client)
{
	std::vector<std::string>	parameters = parseParams(message.substr(message.find(" ")));
    std::vector<std::string>	channels;
    std::string                 temp;
    std::string                 partMessage = client->getNickname();
    std::stringstream			ss2(parameters[0]);

    if (parameters.size() == 2)
    {
        partMessage = parameters[1].substr(1);
    }
	while (std::getline(ss2, temp, ','))
	{
		if (!temp.empty())
			channels.push_back(temp);
	}
    for (size_t i = 0; i < channels.size(); ++i)
    {
        if (!findChannel(channels[i]))
		    client->reply(ERR_NOSUCHCHANNEL(client->getNickname(), channels[i]));
        else if (!(findChannel(channels[i])->findClientInChannel(client->getNickname())))
		    client->reply(ERR_NOTONCHANNEL(client->getNickname(), channels[i]));
        else
        {
            findChannel(channels[i])->deleteChannelClient(client);
            client->reply(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getIPaddress() \
	        + " PART " + channels[i] + " :" + partMessage + "\r\n");
        }
    }
}
