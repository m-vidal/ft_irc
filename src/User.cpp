/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:55:00 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/27 15:13:31 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User() : _nick("~NA"), _name("~NA") {}
User::User(std::string name, std::string nick) : _nick(nick), _name(name) {}

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
