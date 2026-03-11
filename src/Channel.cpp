/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 17:48:29 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/09 00:00:00 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/Channel.hpp"
# include "../inc/User.hpp"
# include <string>
# include <iostream>

Channel::Channel(const std::string &name): _name(name), _topic("") { }

bool	Channel::isUser(int fd) const {
	return (_userFds.find(fd) != _userFds.end());
}

bool	Channel::isOperator(int fd) const {
	return (_operatorFds.find(fd) != _operatorFds.end());
}

bool	Channel::isUser(const User &user) const {
	return (isUser(user.getFd()));
}

bool	Channel::isOperator(const User &user) const {
	return (isOperator(user.getFd()));
}

void	Channel::addUser(int fd) {
	_userFds.insert(fd);
	std::cout << "fd " << fd << " is now a member of " << getName() << "." << std::endl;
}

void	Channel::addOperator(int fd) {
	_operatorFds.insert(fd);
	std::cout << "fd " << fd << " is now an operator of " << getName() << "." << std::endl;
}

void	Channel::removeUser(int fd) {
	if (_userFds.erase(fd))
		std::cout << "fd " << fd << " just left " << getName() << "." << std::endl;
}

void	Channel::removeOperator(int fd) {
	if (_operatorFds.erase(fd))
		std::cout << "fd " << fd << " is not an operator of " << getName() << " anymore." << std::endl;
}

std::string	Channel::getTopic(void) const {
	return (_topic);
}

std::string	Channel::getName(void) const {
	return (_name);
}

// Builds nick list using the server's live _users map to avoid stale data
std::string	Channel::getUserNickList(const std::map<int, User> &allUsers) const {
	std::string	nickList;

	for (std::set<int>::const_iterator it = _userFds.begin(); it != _userFds.end(); ++it) {
		std::map<int, User>::const_iterator uit = allUsers.find(*it);
		if (uit == allUsers.end())
			continue;
		if (isOperator(*it))
			nickList += "@";
		nickList += uit->second.getNick() + " ";
	}
	return (nickList);
}

size_t	Channel::getUserCount(void) const {
	return (_userFds.size());
}

const std::set<int>	&Channel::getUserFds(void) const {
	return (_userFds);
}

