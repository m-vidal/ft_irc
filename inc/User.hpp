/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:54:53 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/02 15:44:25 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>
//--------------------------------------------------
// #include "Channel.hpp"

class User
{
private:
	std::string _nick;
	std::string _name;
	std::string _hostname;
	// std::vector<Channel> _channels;

public:
	User();
	User(std::string name, std::string nick);
	// getters
	const std::string getNick() const;
	const std::string getName() const;
	const std::string getHost() const;
	// setters
	void setNick(const std::string &nick);
	void setName(const std::string &name);
	void setHost(const std::string &hostname);
};

#endif
