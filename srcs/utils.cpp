#include "Server.hpp"

const time_t	getTimestamp()
{
	time_t	timestamp;
	time(&timestamp);
	return timestamp;
}

std::vector<std::string>	parseParams(const std::string& src)
{
	std::vector<std::string>	result;

	std::string					buffer;
	std::string					str = src;
	size_t 						pos = src.find(":");
	if (pos != std::string::npos)
		str = str.substr(0, pos);
	std::stringstream			ss(str);
	while (ss >> buffer)
	{
		result.push_back(buffer);
	}
	if (pos != std::string::npos)
		result.push_back(src.substr(pos));
	//print result
	for (std::vector<std::string>::iterator it = result.begin(); it != result.end(); ++it)
	{
		std::cout << *it << std::endl;
	}
	return result;
}

