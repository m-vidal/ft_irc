/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 18:18:40 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/05 17:41:02 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Parser.hpp"
#include <cstddef>

Parser::Parser(void) { }
//just for testing
Parser::Parser(std::string s) {
	User u;
	this->receiveLine(u, s);
}
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
	const std::size_t ending = line.find("\r\n");
	std::string trimmed = line.substr(0, ending);

	std::size_t commaPos = line.find(":");
	std::string	trailing;

	if (commaPos != std::string::npos){
		trailing = line.substr(commaPos + 1); //trailing
	}
	trailing = "\0";
	std::string	tempArgs = line.substr(0, commaPos); //COMMAND param1 param2 ....

	std::string					command;
	std::vector<std::string>	params;
	short						i = 0;
	
	
	std::size_t spacePos = tempArgs.find(' ');
	command = tempArgs.substr(0, spacePos);

	while (spacePos != std::string::npos) {
		spacePos = tempArgs.find(' ', spacePos + 1);
		params[i++] = tempArgs.substr(0, spacePos);
	}
	std::cout << "command: " << command << std::endl;
	std::cout << "params: " << command;
	std::size_t paramSize = params.size();
	for (size_t i = 0; i < paramSize; i++) {
		std::cout << params[i] << std::endl;
	}
	std::cout << "trailing: " << trailing << std::endl;
	std::cout << "user: " << user.getUser() << std::endl;
}
