/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:05:41 by atambo            #+#    #+#             */
/*   Updated: 2026/02/24 12:12:32 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

User	*Server::getUser(std::string &user_name){
	std::list<User>::iterator it = _users.begin();
	while(it != _users.end())
	{
		if (it.getName() == user_name)
			return it;
	}
	return NULL;
}

Channl	*Server::getChannel(std::string &channel_name){
	std::list<Channel>::iterator it = _channels.begin();
	while(it != _channels.end())
	{
		if (it.getName() == channel_name)
			return it;
	}
	return NULL;
}
