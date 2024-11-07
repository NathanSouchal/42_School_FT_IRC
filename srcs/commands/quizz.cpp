#include "Server.hpp"
#include "Client.hpp"
#include "utils.hpp"

int randomDigitGenerator(std::vector<int> usedIndex)
{
    int randomNb;

    std::srand(std::time(0));
    randomNb = std::rand() % 10;
    while (std::find(usedIndex.begin(), usedIndex.end(), randomNb) != usedIndex.end())
        randomNb = std::rand() % 10;
    return randomNb;
}

void    Server::quizz(const std::string& message, Client *client)
{
    std::vector<std::string> parameters = parseParams(message.substr(message.find(" ")));
    Client  *bot = findClientByNickname("JulienLepers");
    long    nbQuestions;
    int     questionIndex;
    char    *end;

    if (parameters.size() > 1 || parameters[0] > "10" || parameters[0] < "1")
        return client->reply("ERROR: please enter just a number between 1 and 10");
    nbQuestions = strtol(parameters[0].c_str(), &end, 10);
    questionIndex = randomDigitGenerator(client->getUsedIndex());
    client->getUsedIndex().push_back(questionIndex);
    client->setNbQuestions(nbQuestions - 1);
    client->setLastQuestion(quizDataBase[questionIndex]);
    privmsg(" " + client->getNickname() + " :" + quizDataBase[questionIndex][0], bot);
}