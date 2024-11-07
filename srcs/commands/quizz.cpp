#include "Server.hpp"
#include "Client.hpp"

void    Server::quizz(const std::string& message, Client *client)
{
    (void)message;
    std::cout << client->getNickname() << std::endl;
    privmsg(" " + client->getNickname() + " :coucou c'est juju", findClientByNickname("JulienLepers"));
}

std::vector<std::vector<std::string> >	Server::parseQuizDatabase(const std::string& src)
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
	return quizDatabase;
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
