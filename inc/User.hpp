/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:54:53 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/25 11:50:41 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>
#include <map>
#include <algorithm>

class Channel;
class User
{
private:
	bool _isAuthenticated;
	bool _isPassAccepted;
	bool _isNickSet;
	bool _isUserSet;
	std::string _realName;
	std::vector<std::string> _channels;
	std::map<std::string, time_t> _invitations;
	std::string _nickname;
	std::string _username;
	std::string _inbuffer;
	std::string _outbuffer;

	short _fd;
	std::string _hostname;

public:
	User(int fd, const std::string &ip);
	User(void);
	~User(void);
	User(const User &src);
	User &operator=(const User &src);

	// getters
	const std::string &getHostname(void) const;
	const std::string &getUsername(void) const;
	const std::string &getRealname(void) const;
	const std::string &getInbuff(void) const;
	const std::string &getNick(void) const;
	size_t getChannelCount() const;
	int getFd(void) const;
	const std::vector<std::string> &getChannels(void) const;

	// setters
	void setNick(const std::string &nick);
	void setUser(const std::string &user);
	void setRealname(const std::string &name);
	void setIsAuthenticated(void);
	void setPassAccepted(void);

	// checks
	bool checkIsPassAccepted(void) const;
	bool checkIsNickSet(void) const;
	bool checkIsUserSet(void) const;
	bool isAuthenticated(void) const;

	// regular methods
	void authenticate();
	void clearInbuff(size_t pos);
	void appendInbuff(std::string data);

	void addChannel(std::string channel_name);
	void removeChannel(std::string channel_name);

	void addInvite(const std::string &channelName, time_t time);
	void removeInvite(const std::string &channelName);
	bool isInvited(const std::string &channelName);
	std::map<std::string, time_t> &getInvites();

	std::string getPrefix() const;

	void appendOutbuff(const std::string &msg);
	std::string &getOutbuff();
	void clearOutbound(size_t n);
};

#endif
