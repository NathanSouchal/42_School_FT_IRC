/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numerics.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouchal <nsouchal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:09:25 by nsouchal          #+#    #+#             */
/*   Updated: 2024/10/17 15:26:29 by nsouchal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMERICS_HPP
# define NUMERICS_HPP

# include "Server.hpp"

#define ERR_NEEDMOREPARAMS(source, command) ("461 " + std::string(source) + " " + std::string(command) + " :Not enough parameters\r\n")
#define ERR_NOTREGISTERED(source, command) ("451 " + std::string(source) + " " + std::string(command) + " :You have not registered\r\n")
#define ERR_ALREADYREGISTERED(source, command) ("462 " + std::string(source) + " " + std::string(command) + " :You may not reregister\r\n")
#define ERR_PASSWDMISMATCH(source, command) ("464 " + std::string(source) + " " + std::string(command) + " :Password incorrect\r\n")
#endif