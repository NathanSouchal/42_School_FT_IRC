#ifndef UTILS_HPP
# define UTILS_HPP
# include <cstring>
# include <iostream>
# include <sstream>
# include <vector>

class Client;

time_t						        getTimestamp();
std::string	                        getTimestampDate();
std::vector<std::string>	        parseParams(const std::string& src);

template <typename T> std::string   convertInString(T toConvert)
{
    std::ostringstream oss;

    oss << toConvert;
    return (oss.str());
}
bool	                            checkChannelName(std::string name, Client *client);

#endif
