/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:32:05 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/06 14:22:50 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <string>
#include <string>
#include <iostream>

class User;

class Channel
{

public:
	Channel() {};
	~Channel() {};
	Channel(const std::string &name);

	std::string getName(void) const;
	std::string getUserNickList(void) const;
	size_t getUserCount(void) const;
	std::map<int, User> getUsers(void) const;

	void setName(const std::string &newName);

	void addUser(const User &user);
	void addOperator(const User &user);

	void removeUser(const int &fd);
	void removeOperator(const int &fd);

	bool isUser(const User &user) const;
	bool isOperator(const User &user) const;

	void incUsers(void);
	void decUsers(void);
	void incOperators(void);
	void decOperators(void);

private:
	std::string _name;
	std::map<int, User> _users;
	std::string _topic;
	size_t _nbUsers;
	std::map<int, User> _operators;
	size_t _nbOperators;
	// modes
};

#endif
