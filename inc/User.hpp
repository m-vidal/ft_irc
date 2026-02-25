/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:54:53 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/25 11:40:33 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>
//--------------------------------------------------
#include "inc/Channel.hpp"

class Channel
{
};

class User
{
private:
	const short _fd;
	std::string _nick;
	std::string _name;
	std::vector<Channel> _channels;
	std::string _inbuf;
	std::string _outbuf;

public:
	User::User(short fd, std::string name, std::string nick);
	// getters
	const std::string getNick() const;
	const std::string getName() const;
	// setters
	void setNick(const std::string &nick);
	void setName(const std::string &name);
	// ------------------------------------------
	void writeBuffer(std::string data);
	void readBuffer(std::string data);
};

#endif
