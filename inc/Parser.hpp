/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 17:57:24 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/05 17:39:57 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

#include <cstddef>
#include <string>
#include <vector>
#include "User.hpp"

class Parser {

	public:
		Parser();
		~Parser();
		Parser(const Parser &other);
		Parser &operator=(const Parser &other);
		//just for testing
		Parser(std::string s);

		void		receiveLine(User &user, std::string &line);
		std::string	getLine(void);
		void		setLine(std::string line);

	private:
		std::string _line;
};

#endif
