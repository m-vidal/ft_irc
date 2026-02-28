/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:55:00 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/28 17:27:17 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"

User::User(int fd): _isAuthenticated(false),  _interactions(0), _stepOne(false), _fd(fd)
{ _username = "default"; _nickname = "default"; }

User::User(void): _isAuthenticated(false),  _interactions(0), _stepOne(false), _fd(-2)
{ _username = "default"; _nickname = "default";  }

User::~User(void) { }


void User::setNick(const std::string &nick) { this->_nickname = nick; }

void User::setUser(const std::string &user) {this->_username = user; }


size_t	User::getInteractions( void ) { return (_interactions); }
void	User::incInteractions() { _interactions++; }


bool	User::isAuthenticated() { return (_isAuthenticated); }
void	User::authenticate() { _isAuthenticated = true; }


const std::string	&User::getUsername(void) const { return (this->_username); }

const std::string	&User::getNick(void) const { return (this->_nickname); }


bool User::isOperator(void) const { return (this->_isOperator); }


void	User::appendToBuffer(std::string data) { _buffer += data; }

void	User::clearBuffer( size_t pos ) { _buffer.erase(0, pos); }

std::string	User::getBuffer( void ) { return (_buffer); }


int	User::getFd(void) { return (_fd); }


bool	User::getStepOne() { return(_stepOne); }
void	User::setStepOne() { _stepOne = true; }