/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:55:00 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/25 11:51:01 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"

User::User(int fd, const std::string &ip) : _isAuthenticated(false), _isPassAccepted(false), _isNickSet(false),
											_isUserSet(false), _fd(fd), _hostname(ip)
{
	_nickname = "*";
	_username = "~NA";
	_realName = "~NA";
}

User::User(void) : _isAuthenticated(false), _isPassAccepted(false), _isNickSet(false),
				   _isUserSet(false), _fd(-1), _hostname("")
{
	_nickname = "*";
	_username = "~NA";
	_realName = "~NA";
}

User::User(const User &src)
{
	*this = src; // We leverage the assignment operator here to avoid duplication
}

User &User::operator=(const User &src)
{
	if (this != &src) // Protection against self-assignment (u1 = u1)
	{
		this->_fd = src._fd;
		this->_hostname = src._hostname;
		this->_nickname = src._nickname;
		this->_username = src._username;
		this->_realName = src._realName;
		this->_isAuthenticated = src._isAuthenticated;
		this->_isPassAccepted = src._isPassAccepted;
		this->_isNickSet = src._isNickSet;
		this->_isUserSet = src._isUserSet;
	}
	return *this;
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
const std::string &User::getBuffer(void) const { return (_inbuffer); }
const std::string &User::getNick(void) const { return (this->_nickname); }
const std::string &User::getUsername(void) const { return (this->_username); }
const std::string &User::getRealname(void) const { return (this->_realName); }
const std::string &User::getHostname(void) const { return (this->_hostname); }

void User::appendToBuffer(std::string data) { _inbuffer += data; }
void User::clearBuffer(size_t pos) { _inbuffer.erase(0, pos); }

User::~User(void) {}

void User::addChannel(std::string channel_name) { _channels.push_back(channel_name); }

void User::removeChannel(std::string channel_name) {
    for (std::vector<std::string>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if (*it == channel_name) {
            _channels.erase(it);
            return ; // Exit once found to avoid iterator issues
        }
    }
}


void User::addInvite(const std::string &channelName, time_t time)
{
	// If they are already invited, this just updates the time
	_invitations[channelName] = time;
	;
}

void User::removeInvite(const std::string &channelName)
{
	_invitations.erase(channelName);
}

bool User::isInvited(const std::string &channelName)
{
	return _invitations.find(channelName) != _invitations.end();
}

std::map<std::string, time_t> &User::getInvites()
{
	return _invitations;
}

std::string User::getPrefix() const
{
	return _nickname + "!" + _username + "@" + _hostname;
}

size_t User::getChannelCount() const
{
	return _channels.size();
}

const std::vector<std::string> &User::getChannels(void) const
{
	return _channels;
}
