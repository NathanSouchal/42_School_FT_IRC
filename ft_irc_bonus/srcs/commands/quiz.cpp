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

void	Server::sendQuestion(Client *client, Client *bot)
{
	int questionIndex = randomDigitGenerator(client->getUsedIndex());
	client->getUsedIndex().push_back(questionIndex);
	client->setNbQuestions(client->getNbQuestions() - 1);
	client->setLastQuestion(_quizDataBase[questionIndex]);
	privmsg(" " + client->getNickname() + " :" + _quizDataBase[questionIndex][0], bot);
	privmsg(" " + client->getNickname() + " :" + _quizDataBase[questionIndex][1], bot);
	privmsg(" " + client->getNickname() + " :" + _quizDataBase[questionIndex][2], bot);
	privmsg(" " + client->getNickname() + " :" + _quizDataBase[questionIndex][3], bot);
}

void    Server::quiz(const std::string& message, Client *client)
{
    std::vector<std::string> parameters = parseParams(message.substr(message.find(" ")));
    Client  *bot = findClientByNickname("JulienLepers");
    long    nbQuestions;
    char    *end;

	if (client->getNbQuestions())
		return client->reply("ERROR: One game is already running\r\n");
	nbQuestions = strtol(parameters[0].c_str(), &end, 10);
	if (parameters.size() > 1 || nbQuestions > 10 || nbQuestions < 1)
		return client->reply("ERROR: Please enter a number between 1 and 10\r\n");
	client->setNbQuestions(nbQuestions);
	sendQuestion(client, bot);
}

void	Server::parseQuizDatabase(const std::string& src)
{
	std::vector<std::vector<std::string> >	quizDatabase;
	std::vector<std::string>				question;
	std::string							line;
	std::ifstream						file;
	int	count = 0;

	file.open(src.c_str());
	if (!file.is_open())
		throw std::runtime_error("Erreur : Impossible to open file quizzdatabase.txt");
	else
	{
		while (std::getline(file, line))
		{
			if (!line.empty())
			{
				question.push_back(line);
				count++;
				if (count == 5)
				{
					quizDatabase.push_back(question);
					question.clear();
					count = 0;
				}
			}
		}
		file.close();
	}
	_quizDataBase = quizDatabase;
}
