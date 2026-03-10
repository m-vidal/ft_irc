/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:55:00 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/10 13:27:06 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"

User::User(int fd, const std::string &ip) : _isAuthenticated(false), _isPassAccepted(false), _isNickSet(false),
											_isUserSet(false), _fd(fd), _hostname(ip)
{
	_nickname = "~NA";
	_username = "~NA";
	_realName = "~NA";
}

User::User(void) : _isAuthenticated(false), _isPassAccepted(false), _isNickSet(false),
				   _isUserSet(false), _fd(-1), _hostname(0)
{
	_nickname = "~NA";
	_username = "~NA";
	_realName = "~NA";
}

void User::setPassAccepted(void) { _isPassAccepted = true; }
void User::setIsAuthenticated(void) { _isAuthenticated = true; }
void User::setRealname(const std::string &name) { this->_realName = name; }
void User::setUser(const std::string &user)
{
	this->_username = user;
	_isUserSet = true;
}
void User::setNick(const std::string &nick)
{
	this->_nickname = nick;
	_isNickSet = true;
}

bool User::checkIsUserSet(void) const { return (_isUserSet); }
bool User::checkIsNickSet(void) const { return (_isNickSet); }
bool User::checkIsPassAccepted(void) const { return (_isPassAccepted); }

void User::authenticate() { _isAuthenticated = true; }
bool User::isAuthenticated() const { return (_isAuthenticated); }

int User::getFd(void) const { return (_fd); }
std::string User::getBuffer(void) const { return (_inbuffer); }
std::string User::getNick(void) const { return (this->_nickname); }
std::string User::getUsername(void) const { return (this->_username); }
std::string User::getRealname(void) const { return (this->_realName); }
std::string User::getHostname(void) const { return (this->_hostname); }

void User::appendToBuffer(std::string data) { _inbuffer += data; }
void User::clearBuffer(size_t pos) { _inbuffer.erase(0, pos); }

User::~User(void) {}
