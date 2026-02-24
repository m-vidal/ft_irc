/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 18:48:42 by atambo            #+#    #+#             */
/*   Updated: 2026/02/24 15:30:29 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/Executer.hpp"

Executor::Executor(Server& server) : _server(server) {}

void Executor::Topic(User& sender, const std::vector<std::string>& params){
	/*	/topic <channel> <topic>
	 *	check channel param
	 *	check that this channel exists
	 *	check if user is in channel
	 *	check if privilages needed and if so check has priviledge
	 *	check topic string
	*/
	std::string sender_name;
	std::string channel_name = params.at(1);
	Channel *channel = _server.getChannel(channel_name);
	if (channel == NULL)
		// return reply no such channel
	sender_name = sender.getName();
	if (channel->isMember(sender_name) == false)
		// return not channel memeber
	if (channel->hasMode(ModeFlags::TOPIC_PRIV)){
			// return need privilages
	}
	if (checkTopic(params.at(2)) == false){
		// return invalid topic string
	}
	channel->setTopic(params.at(2));

}
