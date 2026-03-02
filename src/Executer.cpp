/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 18:48:42 by atambo            #+#    #+#             */
/*   Updated: 2026/03/02 14:13:18 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Executer.hpp"

Command::Command() : handler(NULL), minArgs(0), requiresAuth(false) {}
Command::Command(Handler h, int args, bool auth) : handler(h), minArgs(args), requiresAuth(auth) {}

Executer::Executer(Server &server) : _server(server)
{
	// Initialize command map

	_cmdHandlers["JOIN"] = Command(&Executer::join, 0, false);
	_cmdHandlers["PRIVMSG"] = Command(&Executer::privmsg, 0, false);
	_cmdHandlers["TOPIC"] = Command(&Executer::topic, 0, false);
}

void Executer::processMessage(User *user, std::string rawCommand)
{
	if (user == NULL)
		return;
	_user = user;
	parser(rawCommand);
	std::cout << "----------------------------------------------------------" << "\n";
	std::cout << "command : " << _cmd << "\n";
	for (size_t i = 0; i < _params.size(); i++)
	{
		std::cout << "param [" << i << "] : " << _params[i] << "\n";
	}
	std::cout << "trailing : " << _trailing << "\n";
	std::cout << "----------------------------------------------------------" << "\n";

	// std::map<std::string, Command>::iterator it = _cmdHandlers.find(_cmd);
	// if (it != _cmdHandlers.end())
	// {
	// 	// 'it->second' is the Command object
	// 	Command cmdInfo = it->second;

	// 	// EXECUTION: You must use 'this' and the dereference operator
	// 	(this->*(cmdInfo.handler))();
	// }
	// else
	// {
	// 	unknowCmd();
	// }
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

void Executer::unknowCmd() {}

void Executer::join() {}

void Executer::privmsg() {}

void Executer::topic() {}
