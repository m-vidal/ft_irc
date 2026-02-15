/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:55:00 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/15 16:57:52 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"

User::User(void): _fd(1) { _username = "default";}
User::~User(void) { }

User::User(const User &other): _fd(other._fd)
{
	_isOperator = other._isOperator;
	_ipAddress = other._ipAddress;
	_username = other._username;
	_nickname = other._nickname;
	_channels = other._channels;
}

void User::setNick(const std::string &nick) {
	this->_nickname = nick;
}

void User::setUser(const std::string &user) {
	this->_username = user;
}

const std::string &User::getNick(void) const {
	return (this->_nickname);
}

const std::string &User::getUsername(void) const {
	return (this->_username);
}

bool User::isOperator(void) const {
	return (this->_isOperator);
}
