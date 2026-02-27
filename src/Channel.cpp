/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 00:34:17 by atambo            #+#    #+#             */
/*   Updated: 2026/02/27 13:56:44 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// Channel::Channel(const User &exec, const std::string &name)
// {
// 	if (checkServerName(name) == false)
// 		throw(std::runtime_error());
// 	this.name = name;
// 	Member creator = {exec, true};
// 	this.membs.push_back = creator;
// 	this.topic = "";
// 	this.maxmeb = 0;
// 	this.modes = 0;
// 	this.passwd = "";
// }

// // --------------------
// // getters
// const std::string &Channel::getName(void) const { return _name; }
// const std::string &Channel::getTopic(void) const { return _topic; }
// const unsigned int &Channel::getMaxMemb(void) const { return _limit; }
// const unsigned short &Channel::getModes(void) const { return _modes; }

// bool Channel::isMember(const std::string &user_name) const
// {
// 	for (std::list<Member>::const_iterator it = _membs.begin(); it != _membs.end(); ++it)
// 	{
// 		if (it->user.getName() == user_name)
// 			return true;
// 	}
// 	return false;
// }
// bool Channel::isOperator(const User &user) const
// {
// 	for (std::list<Member>::const_iterator it = _membs.begin(); it != _membs.end(); ++it)
// 	{
// 		if ((*it)->user == user && it->isoperator)
// 			return true;
// 	}
// 	return false;
// }
// bool Channel::isInvited(const User &user) const
// {
// 	for (std::vector<User>::const_iterator it = invites.begin(); it != invites.end(); ++it)
// 	{
// 		if (*it == user)
// 			return true;
// 	}
// 	return false;
// }
// // setters
// void Channel::setName(const std::string &name) { _name = name; }
// void Channel::setTopic(const std::string &topic) { _topic = topic; }
// void Channel::setLimit(unsigned int limit) { _limit = limit; }
// void Channel::setKey(const std::string &key) { _key = key; }
// void Channel::addMember(User &user) { _membs.push_back(Member{user, false}); }

// void Channel::removeMember(const std::string &user_name)
// {
// 	for (std::list<Member>::iterator it = _membs.begin(); it != _membs.end(); ++it)
// 	{
// 		if (it->user.getName() == user_name)
// 		{
// 			membs.erase(it);
// 			return;
// 		}
// 	}
// }

// void Channel::addInvite(const User &user) { _invited.push_back(user); }
// void Channel::removeInvite(const User &user)
// {
// 	for (std::list<User>::iterator it = _invited.begin(); it != _invited.end(); ++it)
// 	{
// 		if (*it == user)
// 		{
// 			_invited.erase(it);
// 			return;
// 		}
// 	}
// }
// //------------------------------------------------
// void setMode(unsigned char mode);
// void unsetMode(unsigned char mode);
// bool hasMode(unsigned char mode) const;
// //---------------------------------------------