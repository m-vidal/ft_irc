/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:55:00 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/28 21:15:07 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"

User::User(int fd): _isAuthenticated(false),  _interactions(0), _stepOne(false), _fd(fd) { }

User::User(void): _isAuthenticated(false),  _interactions(0), _stepOne(false), _fd(-2) { }

User::~User(void) { }

void User::setNick(const std::string &nick) { this->_nickname = nick; _isNickSet = true; }

void User::setUser(const std::string &user) {this->_username = user; _isUserSet = true; }
void User::setRealname(const std::string &name) {this->_realName = name; }

bool	User::checkIsUserSet(void) const { return(_isUserSet); }
bool	User::checkIsNickSet(void) const { return(_isNickSet); }
bool	User::checkIsPassAccepted(void) const { return(_isPassAccepted); }

size_t	User::getInteractions( void ) const { return (_interactions); }
void	User::incInteractions() { _interactions++; }

void	User::setHostname(const std::string &user) {	this->_hostname = user; }

bool	User::isAuthenticated() const { return (_isAuthenticated); }
void	User::authenticate() { _isAuthenticated = true; }

std::string User::getUsername(void) const { return (this->_username); }
std::string User::getRealname(void) const { return (this->_realName); }

std::string	User::getNick(void) const { return (this->_nickname); }

std::string	User::getHostname(void) const { return (this->_hostname); }

void		User::setPassAccepted(void) {
	_isPassAccepted = true;
}

void		User::setIsAuthenticated(void) {
	_isAuthenticated = true;
}

bool User::isOperator(void) const { return (this->_isOperator); }


void	User::appendToBuffer(std::string data) { _buffer += data; }

void	User::clearBuffer( size_t pos ) { _buffer.erase(0, pos); }

std::string	User::getBuffer( void ) const { return (_buffer); }


int	User::getFd(void) const { return (_fd); }


bool	User::getStepOne() const { return(_stepOne); }
void	User::setStepOne() { _stepOne = true; }
