/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:54:53 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/26 18:49:19 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>
//--------------------------------------------------
#include "Channel.hpp"

class User
{
private:
	std::string _nick;
	std::string _name;
	std::vector<Channel> _channels;

public:
	User(short fd, std::string name, std::string nick);
	// getters
	const std::string getNick() const;
	const std::string getName() const;
	// setters
	void setNick(const std::string &nick);
	void setName(const std::string &name);
};

#endif
