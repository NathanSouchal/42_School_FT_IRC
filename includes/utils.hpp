/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnicolau <tnicolau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 15:01:56 by nsouchal          #+#    #+#             */
/*   Updated: 2024/10/23 13:25:14 by tnicolau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP
# include <cstring>
# include <iostream>
# include <sstream>

time_t						getTimestamp();
std::vector<std::string>	parseParams(const std::string& src);

template <typename T> std::string   convertInString(T toConvert)
{
    std::ostringstream oss;

    oss << toConvert;
    return (oss.str());
}

#endif
