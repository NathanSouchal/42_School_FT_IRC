/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouchal <nsouchal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 15:01:56 by nsouchal          #+#    #+#             */
/*   Updated: 2024/10/22 15:07:08 by nsouchal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP
# include <cstring>
# include <iostream>
# include <sstream>

std::string	                        getTimestamp();
template <typename T> std::string   convertInString(T toConvert)
{
    std::ostringstream oss;

    oss << toConvert;
    return (oss.str());
}

#endif