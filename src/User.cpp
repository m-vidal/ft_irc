/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:55:00 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/25 11:39:36 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"

User::User(short fd, std::string name, std::string nick) : _fd(fd), _nick(nick), _name(name) {}

void User::setNick(const std::string &nick)
{
	this->_nick = nick;
}

// getters
const std::string User::getNick() const { return _nick; }

const std::string User::getName() const { return _name; }

// setters
void User::setNick(const std::string &nick)
{
	this->_nick = nick;
}

void User::setName(const std::string &name)
{
	this->_name = name;
}
