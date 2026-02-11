/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 17:57:24 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/09 19:03:10 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <string>
# include <cctype>
# include <iostream>

# include "User.hpp"

class Parser {

	public:
		Parser();
		~Parser();
		//just for testing
		Parser(User &user, std::string &str);


	private:
		std::vector<std::string>	split(const std::string& input);
		void						receiveLine(User &user, std::string &line);
};

#endif
