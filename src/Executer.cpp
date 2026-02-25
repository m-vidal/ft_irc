/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 18:48:42 by atambo            #+#    #+#             */
/*   Updated: 2026/02/24 17:12:08 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/Executer.hpp"

Executor::Executor(Server& server) : _server(server){
	// Initialize command map
	_cmd["JOIN"] = &Executor::Join;
	_cmd["PRIVMSG"] = &Executor::Privmsg;
	_cmd["TOPIC"] = &Executor::Topic;
}

std::vector<std::string>	Executor::parser(std::string &rawCommand) {
	/* split rawCommand into command and params
	 * return vector of command and params
	*/
	std::vector<std::string> result;
	std::istringstream iss(rawCommand);
	std::string token;
	while (iss >> token) {
		result.push_back(token);
	}
	return result;
}

void Executor::execute(User& sender, std::string rawCommand) {
    std::vector<std::string> params = parser(rawCommand);
    if (params.empty())
        return;

    std::string command = params[0];

    // Look for the command in the map
    auto it = _cmd.find(command);

    if (it != _cmd.end()) {
        // Syntax for calling a member function pointer: (instance->*pointer)(args)
        CommandHandler handler = it->second;
        (this->*handler)(sender, params);
    } else {
        // Handle unknown command (e.g., send ERR_UNKNOWNCOMMAND to client)
    }
}

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
