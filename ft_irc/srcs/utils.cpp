#include "utils.hpp"

time_t	getTimestamp()
{
	time_t	timestamp;
	time(&timestamp);
	return timestamp;
}

std::string	getTimestampDate()
{
	time_t	timestamp;
	time(&timestamp);
	std::string timeString = ctime(&timestamp);
	return timeString;
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
	return result;
}
