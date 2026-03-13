/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:32:05 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/13 10:03:10 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <set>
#include <ctime>
#include <string>
#include <sstream>
#include <iostream>

#define CHAN_NAME_MAX = 200;

class User;
struct Member
{
	bool is_operator;
	User user;

	Member();
	Member(bool op, User user);
};

struct TopicData
{
	std::string content; // The actual topic text
	std::string setBy;	 // Nickname of the user who set it
	time_t creationTime; // Unix timestamp
};
class Channel
{

public:
	~Channel();
	Channel(const std::string &name);
	// do not add default constructor use find istead
	const std::string &getName(void) const;
	std::string getMemberNickList(void) const;
	const std::map<int, Member> &getMembers(void) const;
	size_t getMemberCount(void) const;

	void setName(const std::string &newName);

	void addMember(User &user);
	void addOperator(const User &member);

	void removeMember(const int &fd);

	bool isMember(const int fd) const;
	bool isMember(const User &user) const;
	bool isOperator(const int fd) const;
	bool isOperator(const User &user) const;

	void setOperator(int fd, bool make_operator);
	void setOperator(const User &user, bool make_operator);

	// Channel_mode
	bool isModeChar(const char c);
	void setMode(const char c);
	void unsetMode(const char c);
	std::map<char, bool> getMode() const;
	bool hasMode(const char c) const;
	std::string getModeStr() const;

	std::string getCreationTimeStr() const;

	void setKey(std::string &new_key);
	void unsetKey();
	void initMode();
	bool verifyKey(const std::string user_key) const;

	void addInvite(User &user);
	void removeInvite(User &user);
	bool isInvited(const std::string &user_nick) const;

	void setTopic(const std::string &new_topic, const std::string &setter);
	const TopicData &getTopic() const;

	void setLimit(size_t limit);
	void unsetLimit();
	size_t getLimit() const;

private:
	std::map<char, bool> _mode;
	std::string _name;
	TopicData _topic;
	std::map<int, Member> _members;
	std::map<std::string, time_t> _invited_users;
	time_t _creationTime;
	std::string _key;
	size_t _limit;
};

#endif
