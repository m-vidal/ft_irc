/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 17:48:29 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/03 19:25:30 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/Channel.hpp"
# include "../inc/User.hpp"
# include <string>
# include <iostream>

Channel::Channel(const std::string &name): _name(name), _nbUsers(0), _nbOperators(0) { }

bool	Channel::isUser(const User &user) const {
	std::map<int, User>::const_iterator it = _users.find(user.getFd());
	if (it != _users.end()) {
		return (true);
	}
	return (false);
}
bool	Channel::isOperator(const User &user) const {
	std::map<int, User>::const_iterator it = _operators.find(user.getFd());
	if (it != _operators.end()) {
		return (true);
	}
	return (false);
}


void		Channel::addUser(const User &user) {
	_users.insert(std::make_pair(user.getFd(), user));
	std::cout << "User " << user.getNick() << " is now a member of " << getName() << "." << std::endl;
	incUsers();
}

void		Channel::addOperator(const User &user) {
	_operators.insert(std::make_pair(user.getFd(), user));
	std::cout << "User " << user.getNick() << " is now an operator  of " << getName() << "." << std::endl;
	incOperators();
}

void		Channel::removeUser(const int &fd) {
	std::map<int, User>::iterator it = _users.find(fd);
	if (it != _users.end()) {
		std::cout << "User " << it->second.getNick() << " just left " << getName() << "." << std::endl;
		_users.erase(it);
		decUsers();
	}
}
void		Channel::removeOperator(const int &fd) {
	std::map<int, User>::iterator it = _operators.find(fd);
	if (it != _operators.end()) {
		std::cout << "User " << it->second.getNick() << " is not an operator of " << getName() << " anymore." << std::endl;
		_operators.erase(it);
		decOperators();
	}
}

std::string	Channel::getName(void) const {
	return (_name);
}
std::string	Channel::getUserNickList(void) const {
	std::string	nickList;
	std::map<int, User>::const_iterator it;

    // 3. The Loop
    for (it = _users.begin(); it != _users.end(); ++it) {
		if (isOperator(it->second) == true) {
			nickList += "@";
		}
		nickList += it->second.getNick() + " ";
    }
	return (nickList);
}
size_t	Channel::getUserCount(void) const {
	return (_nbUsers);
}
std::map<int, User>	Channel::getUsers(void) const {
	return (_users);
}

void	Channel::incUsers(void) {
	_nbUsers++;
	std::cout << "Numbers of members of " << getName() << ": " << _nbUsers << "." << std::endl;
}
void	Channel::decUsers(void) {
	if (_nbUsers > 0) 
		_nbUsers--;
	else 
		std::cerr << "Number of users already at zero!" << "Channel name: " << getName() << std::endl;
	std::cout << "Numbers of members of " << getName() << ": " << _nbUsers << "." << std::endl;
}
void	Channel::incOperators(void) {
	_nbOperators++;
	std::cout << "Numbers of members of " << getName() << ": " << _nbUsers << "." << std::endl;
}
void	Channel::decOperators(void) {
	if (_nbOperators > 0)
		_nbOperators--;
	else 
		std::cerr << "Number of users operators at zero!" << "Channel name: " << getName() << std::endl;
	std::cout << "Numbers of members of " << getName() << ": " << _nbUsers << "." << std::endl;
}
