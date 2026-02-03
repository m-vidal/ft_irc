/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 18:18:40 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/03 18:27:38 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Parser.hpp"


Parser::Parser(void) { }
Parser::~Parser(void) { }
Parser &Parser::operator=(const Parser &other) {
	std::cout << "Parser's Copy assignment operator called" << std::endl;
	if (this != &other)
	{
		this->_line = other._line;
	}
	return (*this);
}

Parser::Parser(const Parser &other) {
	std::cout << "Parser's copy constructor operator called" << std::endl;

	*this = other;
}

std::string Parser::getLine(void) {
	return (this->_line);
}

void Parser::setLine(std::string line) {
	this->_line = line;
}

void Parser::receiveLine(User &user, std::string &line) {

}
