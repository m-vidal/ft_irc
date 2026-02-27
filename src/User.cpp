/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:55:00 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/27 17:43:55 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"

User::User(void): _fd(1), _interactions(0) { _username = "default";}
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
void	User::incInteractions()
{
	_interactions++;
}

size_t	User::getInteractions( void ) {
	return (_interactions);
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

void	User::appendToBuffer(std::string data) {
	_buffer += data;
}

std::string	User::getBuffer( void ) {
	return (_buffer);
}

void	User::clearBuffer( size_t pos ) {
	_buffer.erase(0, pos);
}