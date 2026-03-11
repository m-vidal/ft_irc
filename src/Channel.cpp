/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 17:48:29 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/11 17:17:03 by atambo           ###   ########.fr       */
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
Channel::Channel(const std::string &name) : _name(name), _creationTime(std::time(NULL))
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

void Channel::addOperator(const User &user)
{
	_members.insert(std::make_pair(user.getFd(), Member(true, user)));
	std::cout << "User " << user.getNick() << " is now an operator of " << _name << "." << std::endl;
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

void Channel::setOperator(const int fd)
{
	std::map<int, Member>::iterator it = _members.find(fd);
	if (it != _members.end())
	{
		it->second.is_operator = true;
		std::cout << "User " << it->second.user.getNick() << " is now an operator." << std::endl;
	}
}

void Channel::setOperator(const User &user)
{
	this->setOperator(user.getFd());
}

void Channel::unsetOperator(const int fd)
{
	std::map<int, Member>::iterator it = _members.find(fd);
	if (it != _members.end())
	{
		it->second.is_operator = false;
		std::cout << "User " << it->second.user.getNick() << " is no longer an operator." << std::endl;
	}
}

void Channel::unsetOperator(const User &user)
{
	this->unsetOperator(user.getFd());
}

const std::map<int, Member> &Channel::getMembers(void) const
{
	return _members;
}

std::string Channel::getCreationTimeStr() const
{

	return timeToStr(_creationTime);
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
