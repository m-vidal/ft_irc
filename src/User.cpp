/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:55:00 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/05 17:37:05 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"
#include <string>

User::User(void) { _username = "default";}
User::~User(void) { }
User &User::operator=(const User &other) {
	std::cout << "User's Copy assignment operator called" << std::endl;
	if (this != &other)
	{
		this->setNick(other.getNick());
		this->setUser(other.getUser());
	}
	return (*this);
}

User::User(const User &other) {
	std::cout << "User's copy constructor operator called" << std::endl;

	*this = other;
}

void User::setNick(const std::string &nick) {
	this->_nickname = nick;
}

void User::setUser(const std::string &user) {
	this->_username = user;
}

void User::setOperator(bool op) {
	this->_isOperator = op;
}

void User::authenticate(void) {
	this->_authenticated = true;
}

const std::string &User::getNick(void) const {
	return (this->_nickname);
}

const std::string &User::getUser(void) const {
	return (this->_username);
}

bool User::isAuthenticated(void) const {
	return (this->_authenticated);
}

bool User::isOperator(void) const {
	return (this->_isOperator);
}


