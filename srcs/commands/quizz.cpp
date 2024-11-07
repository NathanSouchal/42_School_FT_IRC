#include "Server.hpp"
#include "Client.hpp"

void    Server::quizz(const std::string& message, Client *client)
{
    (void)message;
    std::cout << client->getNickname() << std::endl;
    privmsg(" " + client->getNickname() + " :coucou c'est juju", findClientByNickname("JulienLepers"));
}