/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:54:53 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/10 12:50:12 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>

class User
{
private:
	bool _isAuthenticated;
	bool _isPassAccepted;
	bool _isNickSet;
	bool _isUserSet;
	std::string _realName;
	// std::vector<Channel>	_channels;	//acho que se vai apagar depois
	std::string _nickname;
	std::string _username;
	std::string _inbuffer;
	std::string _outbuffer;

	const short _fd;
	const std::string _hostname;

public:
	User(int fd, const std::string &ip);
	User(void);
	~User(void);

	// getters
	std::string getHostname(void) const;
	std::string getUsername(void) const;
	std::string getRealname(void) const;
	std::string getBuffer(void) const;
	std::string getNick(void) const;
	int getFd(void) const;

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
	void clearBuffer(size_t pos);
	void appendToBuffer(std::string data);
};

#endif
