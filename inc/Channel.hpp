/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:32:05 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/26 17:50:44 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <list>
#include <exception>
//--------------------------------------------------
#include "inc/numReply.hpp"
#include "inc/User.hpp"
#include "inc/Server.hpp"
#include "inc/Executer.hpp"

// mode enums (00000) (i k l o t)
enum ModeFlags
{
	INVITE_ONLY = 1 << 0, // 1  (00001)
	KEY_REQ = 1 << 1,	  // 4  (00010)
	LIMIT_SET = 1 << 2,	  // 16 (00100)
	OPER_ONLY = 1 << 3,	  // 8  (01000)
	TOPIC_PRIV = 1 << 4	  // 2  (10000)
};

class User
{
};

typedef struct Member
{
	User &user;
	bool isoperator;
};

class Channel
{

private:
	std::string _name;
	std::string _topic;
	unsigned int _limit;
	unsigned short _modes;
	std::list<Member> _membs;
	std::list<User &> _invited;
	std::string _key;

	void serverReply();

public:
	Channel(const User &exec, const std::string &name);
	// --------------------
	// getters
	const std::string &getName(void) const;
	const std::string &getTopic(void) const;
	const unsigned int &getMaxMemb(void) const;
	const unsigned short &getModes(void) const;

	bool isMember(const std::string &user_name) const;
	bool isOperator(const User &user) const;
	bool isInvited(const User &user) const;
	// setters
	void setName(const std::string &name);
	void setTopic(const std::string &topic);
	void setLimit(unsigned int limit);
	void setKey(const std::string &key);
	void addMember(User &user);
	void removeMember(const std::string &user_name);
	void addInvite(const User &user);
	void removeInvite(const User &user);
	//------------------------------------------------
	void setMode(unsigned char mode);
	void unsetMode(unsigned char mode);
	bool hasMode(unsigned char mode) const;
	//---------------------------------------------
};

#endif
