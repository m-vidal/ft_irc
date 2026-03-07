/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 17:48:29 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/07 03:54:24 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"

Member::Member() : is_operator(false) {}
Member::Member(bool op, const User user) : is_operator(op), user(user) {}

Channel::Channel(const std::string &name) : _name(name)
{
	applyModeString("+t-ikol");
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

void Channel::addMember(const User &user)
{
	_members.insert(std::make_pair(user.getFd(), Member(false, user)));
	std::cout << "User " << user.getNick() << " joined " << _name << "." << std::endl;
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

std::string Channel::getName(void) const
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

std::map<int, Member> Channel::getMembers(void) const
{
	return _members;
}

void Channel::setMode(const char c)
{
	// Ensure the character is a lowercase letter to avoid overflow
	if (c < 'a' || c > 'z')
		return;
	int index = c - 'a';
	// Use bitwise OR (|) to set the bit to 1
	_mode |= (1 << index);
}

void Channel::unsetMode(const char c)
{
	if (c < 'a' || c > 'z')
		return;
	int index = c - 'a';
	// Use bitwise AND (&) with a bitwise NOT (~) to set the bit to 0
	_mode &= ~(1 << index);
}

unsigned long Channel::getMode() const
{
	return (_mode);
}

bool Channel::hasMode(const char c) const
{
	if (c < 'a' || c > 'z')
		return false;

	int index = c - 'a';
	// Check if the specific bit is 1
	return (_mode & (1 << index));
}

std::string Channel::getModeStr() const
{
	std::string modeStr = "+";

	for (int i = 0; i < 26; i++)
	{
		// Check if the i-th bit is set
		if (_mode & (1 << i))
		{
			// Convert index back to char (0 -> 'a', 1 -> 'b', etc.)
			modeStr += static_cast<char>('a' + i);
		}
	}

	// If no modes are set, some servers return an empty string or just "+"
	// In IRC, it's usually cleaner to return an empty string if only "+" exists
	if (modeStr == "+")
		return "";

	return modeStr;
}

void Channel::applyModeString(const std::string &modes)
{
	bool adding = true;
	unsigned long new_mode = _mode; // Start with the current state

	for (size_t i = 0; i < modes.length(); ++i)
	{
		char c = modes[i];
		if (c == '+')
		{
			adding = true;
			continue;
		}
		if (c == '-')
		{
			adding = false;
			continue;
		}

		// Determine the bit shift based on case
		int shift = -1;
		if (c >= 'a' && c <= 'z')
			shift = c - 'a';
		else if (c >= 'A' && c <= 'Z')
			shift = (c - 'A') + 26;

		if (shift != -1)
		{
			if (adding)
				new_mode |= (1L << shift); // Set bit
			else
				new_mode &= ~(1L << shift); // Clear bit
		}
	}

	_mode = new_mode; // Apply the final result once
}