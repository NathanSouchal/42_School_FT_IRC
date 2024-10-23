/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 09:23:55 by tnicolau          #+#    #+#             */
/*   Updated: 2024/10/23 13:28:17 by tnicolau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

time_t	getTimestamp()
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

