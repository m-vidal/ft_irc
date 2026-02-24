/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 17:57:24 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/21 18:33:21 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <string>
# include <cctype>
# include <iostream>

# include "User.hpp"

class Parser {
	Public:
		Parser(Server &server, User &user, std::string command);
	private:
		std::vector<std::string>	split(const std::string& input);
		void						receiveLine(User &user, std::string &line);
};

#endif
