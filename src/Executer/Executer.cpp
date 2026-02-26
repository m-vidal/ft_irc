/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 18:48:42 by atambo            #+#    #+#             */
/*   Updated: 2026/02/26 19:52:23 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Executer.hpp"

Executer::Executer(Server &server) : _server(server)
{
	// Initialize command map
	_cmdHandlers["JOIN"] = {&Executer::join, 0, false};
	_cmdHandlers["PRIVMSG"] = {&Executer::privmsg, 0, false};
	_cmdHandlers["TOPIC"] = (&Executer::topic, 0, false);
}

//     /privmsg #atambo :

void Executer::execute(User *user, std::string rawCommand)
{
	if (user == NULL)
		return;
	parser(rawCommand);
	auto it = _cmdHandlers.find(_cmd);
	if (it != _cmdHandlers.end())
	{
		Command cmd = it->second.handler;
	}
	else
	{
		unknowCmd();
	}
}

void Executer::privmsg()
{
	if (_trailing.size() < 1)
		return;
	if (_params.size() != 1)
		return;
	std::string target_nick = _params[0];
	User *target = _server.getUser(target_nick);
	if (target == NULL)
		return;
}

// void Executer::Topic(User &sender, const std::vector<std::string> &params)
// {
// 	/*	/topic <channel> <topic>
// 	 *	check channel param
// 	 *	check that this channel exists
// 	 *	check if user is in channel
// 	 *	check if privilages needed and if so check has priviledge
// 	 *	check topic string
// 	 */
// 	std::string sender_name;
// 	std::string channel_name = params.at(1);
// 	Channel *channel = _server.getChannel(channel_name);
// 	if (channel == NULL)
// 		// return reply no such channel
// 		sender_name = sender.getName();
// 	if (channel->isMember(sender_name) == false)
// 		// return not channel memeber
// 		if (channel->hasMode(ModeFlags::TOPIC_PRIV))
// 		{
// 			// return need privilages
// 		}
// 	if (checkTopic(params.at(2)) == false)
// 	{
// 		// return invalid topic string
// 	}
// 	channel->setTopic(params.at(2));
// }
