/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:32:05 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/07 00:06:42 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <string>
#include <set>
#include <iostream>

class User;
struct Member
{
	bool is_operator;
	User user;

	Member();
	Member(bool op, User user);
};

class Channel
{

public:
	Channel() {};
	~Channel() {};
	Channel(const std::string &name);

	std::string getName(void) const;
	std::string getMemberNickList(void) const;
	std::map<int, Member> getMembers(void) const;
	size_t getMemberCount(void) const;

	void setName(const std::string &newName);

	void addMember(const User &user);
	void addOperator(const User &member);

	void removeMember(const int &fd);

	bool isMember(const int fd) const;
	bool isMember(const User &user) const;
	bool isOperator(const int fd) const;
	bool isOperator(const User &user) const;

	void setOperator(const int fd);
	void setOperator(const User &user);
	void unsetOperator(const int fd);
	void unsetOperator(const User &user);

private:
	std::string _name;
	std::string _topic;
	std::map<int, Member> _members;
};

#endif
