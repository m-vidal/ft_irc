/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:32:05 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/10 17:33:54 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <set>
#include <ctime>
#include <string>
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
	static const int MAX_KEY_LEN = 23;

	~Channel();
	Channel();
	Channel(const std::string &name);
	std::string getName(void) const;
	std::string getMemberNickList(void) const;
	std::map<int, Member> getMembers(void) const;
	size_t getMemberCount(void) const;

	void setName(const std::string &newName);

	void addMember(User &user);
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

	// Channel_mode
	bool isModeChar(const char c);
	void setMode(const char c);
	void unsetMode(const char c);
	std::map<char, bool> getMode() const;
	bool hasMode(const char c) const;
	std::string getModeStr() const;

	std::string getCreationTimeStr();

	void setKey(std::string &new_key);
	void unsetKey();
	void initMode();
	bool verifyKey(const std::string user_key) const;

private:
	std::map<char, bool> _mode;
	std::string _name;
	std::string _topic;
	std::map<int, Member> _members;
	std::map<std::string, User> _invited_users;
	time_t _creationTime;
	std::string _key;
};

#endif
