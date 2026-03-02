/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:55:00 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/02 15:46:02 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User() : _nick("~NA"), _name("~NA") {}
User::User(std::string name, std::string nick) : _nick(nick), _name(name) {}

// getters
const std::string User::getNick() const { return _nick; }
const std::string User::getName() const { return _name; }
const std::string User::getHost() const { return _hostname; }

// setters
void User::setNick(const std::string &nick) { this->_nick = nick; }
void User::setName(const std::string &name) { this->_name = name; }
void User::setHost(const std::string &hostname) { this->_hostname = hostname; }
