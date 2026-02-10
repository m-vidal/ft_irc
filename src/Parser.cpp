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

Parser::Parser(void) { }
//just for testing
Parser::Parser(std::string s) {
	User u;
	this->receiveLine(u, s);
}
Parser::~Parser(void) { }

std::string Parser::getLine(void) {
	return (this->_line);
}

void Parser::setLine(std::string line) {
	this->_line = line;
}

void Parser::receiveLine(User &user, std::string &line) {
	std::string trimmed = line.substr(0, line.find("\r\n")); //trimming the \r\n

	std::size_t commaPos = trimmed.find(":");
	std::string	trailing;

	trailing = "\0";
	if (commaPos != std::string::npos){
		trailing = trimmed.substr(commaPos + 1); //trailing
	}
	
	std::string	strBeforeTrailing = trimmed.substr(0, commaPos); //COMMAND param1 param2 ....

	std::string					command;
	std::vector<std::string>	params;

	std::size_t spacePos = strBeforeTrailing.find(' ');
	
	command = strBeforeTrailing.substr(0, spacePos);
	std::size_t commandLength = command.length();
	for (std::size_t i = 0; i < commandLength; i++) {
		command[i] = std::toupper(command[i]);
	}
	std::string tempArgs = strBeforeTrailing.substr(spacePos + 1);
	std::vector<std::string> args = this->split(tempArgs);
}

std::vector<std::string> Parser::split(const std::string& input) {
    std::vector<std::string> result;
    std::string token;
    std::string::size_type i = 0;
    const std::string::size_type n = input.size();

    while (i < n && (input[i] == ' ' || input[i] == '\t'))
        ++i;

    while (i < n) {
        token.clear();
        while (i < n && input[i] != ' ' && input[i] != '\t') {
            token += input[i];
            ++i;
        }

        if (!token.empty())
            result.push_back(token);
        while (i < n && (input[i] == ' ' || input[i] == '\t'))
            ++i;
    }
    return result;
}
