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

    nbQuestions = strtol(parameters[0].c_str(), &end, 10);
    if (parameters.size() > 1 || nbQuestions > 10 || nbQuestions < 1)
        return client->reply("ERROR: please enter just a number between 1 and 10");
    questionIndex = randomDigitGenerator(client->getUsedIndex());
    client->getUsedIndex().push_back(questionIndex);
    client->setNbQuestions(nbQuestions - 1);
    client->setLastQuestion(_quizDataBase[questionIndex]);
    privmsg(" " + client->getNickname() + " :\n" + _quizDataBase[questionIndex][0] + "\n" + \
    _quizDataBase[questionIndex][1] + "\n" + _quizDataBase[questionIndex][2] + "\n" + _quizDataBase[questionIndex][3], bot);
}

void	Server::parseQuizDatabase(const std::string& src)
{
	std::vector<std::vector<std::string> >	quizDatabase;
	std::vector<std::string>				question;
	std::string							line;
	std::ifstream						file;
	int	count = 0;

	std::cout << "Parsing quiz database" << std::endl;
	file.open(src.c_str());
	if (!file.is_open())
		std::cerr << "Erreur : Impossible d'ouvrir le fichier quizzdatabase.txt" << std::endl;
	else
	{
		std::cout << "coucou" << std::endl;
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
	for (size_t i = 0; i < quizDatabase.size(); ++i)
	{
		for (size_t j = 0; j < quizDatabase[i].size(); ++j)
			std::cout << quizDatabase[i][j] << std::endl;
	}
    _quizDataBase = quizDatabase;
}

// {
// 	std::vector<std::vector<std::string> >	quizDatabase;
// 	std::vector<std::string>				question;
// 	std::string							line;
// 	int	count = 0;

// 	int fd = open(database.c_str(), O_RDONLY);
// 	if (fd == -1)
// 		std::cerr << "Erreur : Impossible d'ouvrir le fichier database" << std::endl;
// 	else
// 	{
// 		int flags = fcntl(fd, F_GETFL, 0);
// 		fcntl(fd, F_SETFL, flags | O_NONBLOCK);
// 		char buffer[1024];
// 		ssize_t	bytesRead;
// 		std::string	partialLine;

// 		while (42)
// 		{
// 			bytesRead = read(fd, buffer, sizeof(buffer));
// 			if (bytesRead > 0)
// 			{
// 				std::string	tmp(buffer, bytesRead);
// 				size_t		pos = 0;
// 				while ((pos = tmp.find("\n")) != std::string::npos)
// 				{
// 					line = partialLine + tmp.substr(0, pos);
// 					partialLine.clear();
// 					tmp.erase(0, pos + 1);
// 					if (!line.empty())
// 					{
// 						question.push_back(line);
// 						count++;
// 						if (count == 5)
// 						{
// 							quizDatabase.push_back(question);
// 							question.clear();
// 							count = 0;
// 						}
// 					}
// 				}
// 				partialLine += tmp;
// 			}
// 			else if (bytesRead == 0)
// 				break;
// 			else
// 			{
// 				std::cerr << "Erreur : Impossible de lire le fichier quizzdatabase.txt" << std::endl;
// 				break;
// 			}
// 		}
// 		close(fd);
// 	}
// 	for (size_t i = 0; i < quizDatabase.size(); ++i)
// 	{
// 		for (size_t j = 0; j < quizDatabase[i].size(); ++j)
// 			std::cout << quizDatabase[i][j] << std::endl;
// 	}
// 	return quizDatabase;
// }
