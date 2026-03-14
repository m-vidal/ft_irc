/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 17:48:29 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/14 17:08:18 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

Member::Member() : is_operator(false) {}
Member::Member(bool op, const User user) : is_operator(op), user(user) {}

Channel::~Channel()
{
}

void Channel::initMode()
{
	_mode['i'] = false; // Invite only
	_mode['t'] = true;	// Topic restricted
	_mode['k'] = false; // Key protected
	_mode['l'] = false; // Limit protected
}

Channel::Channel(const std::string &name) : _name(name), _creationTime(std::time(NULL)), _limit(0)
{
	initMode();
}

bool Channel::isMember(const int fd) const
{
	if (_members.find(fd) != _members.end())
		return (true);
	return (false);
}

bool Channel::isMember(const User &user) const
{
	return (isMember(user.getFd()));
}

bool Channel::isOperator(const int fd) const
{
	std::map<int, Member>::const_iterator it = _members.find(fd);
	if (it != _members.end())
	{
		return (it->second.is_operator);
	}
	return (false);
}

bool Channel::isOperator(const User &user) const
{
	return (isOperator(user.getFd()));
}

void Channel::addMember(User &user)
{
	_members.insert(std::make_pair(user.getFd(), Member(false, user)));
	std::cout << "User " << user.getNick() << " joined " << _name << "." << std::endl;
	user.addChannel(this->getName());
}

void Channel::removeMember(const int &fd)
{
	std::map<int, Member>::iterator it = _members.find(fd);
	if (it != _members.end())
	{
		std::cout << "User " << it->second.user.getNick() << " left " << _name << "." << std::endl;
		_members.erase(it);
	}
}

const std::string &Channel::getName(void) const
{
	return (_name);
}

void Channel::setName(const std::string &newName)
{
	_name = newName;
}

std::string Channel::getMemberNickList(void) const
{
	std::string nickList;
	std::map<int, Member>::const_iterator it;

	for (it = _members.begin(); it != _members.end(); ++it)
	{
		if (it->second.is_operator)
			nickList += "@";
		nickList += it->second.user.getNick();

		// Add space if not the last element
		std::map<int, Member>::const_iterator nextIt = it;
		if (++nextIt != _members.end())
			nickList += " ";
	}
	return (nickList);
}

size_t Channel::getMemberCount(void) const
{
	return (_members.size());
}

void Channel::setOperator(int fd, bool make_operator)
{
	std::map<int, Member>::iterator it = _members.find(fd);

	if (it != _members.end())
	{
		it->second.is_operator = make_operator;

		// Dynamic logging based on the toggle
		std::string status = make_operator ? " is now an operator." : " is no longer an operator.";
		std::cout << "User " << it->second.user.getNick() << status << std::endl;
	}
}

void Channel::setOperator(const User &user, bool make_operator)
{
	this->setOperator(user.getFd(), make_operator);
}

const std::map<int, Member> &Channel::getMembers(void) const
{
	return _members;
}

void Channel::addInvite(User &user)
{
	time_t now = std::time(NULL);

	// 1. Update Channel's list
	_invited_users[user.getNick()] = now;

	// 2. Update User's list (Calling the User's method)
	user.addInvite(this->getName(), now);
}

void Channel::removeInvite(User &user)
{
	// 1. Update Channel's list
	_invited_users.erase(user.getNick());

	// 2. Update User's list
	user.removeInvite(this->getName());
}

bool Channel::isInvited(const std::string &user_nick) const
{
	return _invited_users.find(user_nick) != _invited_users.end();
}

void Channel::setTopic(const std::string &new_topic, const std::string &setter)
{
	_topic.content = new_topic;
	_topic.setBy = setter;
	_topic.creationTime = time(NULL);
	std::cout << "new topic = " << _topic.content << "\n";
}

const TopicData &Channel::getTopic() const { return _topic; };

void Channel::setLimit(size_t limit)
{
	this->_limit = limit;
	std::cout << "Channel " << _name << " limit set to " << _limit << std::endl;
}

void Channel::unsetLimit()
{
	this->_limit = 0; // Or whatever default you prefer
	std::cout << "Channel " << _name << " limit removed." << std::endl;
}

size_t Channel::getLimit() const
{
	return this->_limit;
}