/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:55:00 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/09 19:00:07 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"

User::User(void) { _username = "default";}
User::~User(void) { }

void User::setNick(const std::string &nick) {
	this->_nickname = nick;
}

void User::setUser(const std::string &user) {
	this->_username = user;
}


const std::string &User::getNick(void) const {
	return (this->_nickname);
}

const std::string &User::getUser(void) const {
	return (this->_username);
}

bool User::isOperator(void) const {
	return (this->_isOperator);
}


