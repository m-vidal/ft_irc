/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 00:34:17 by atambo            #+#    #+#             */
/*   Updated: 2026/02/21 14:07:21 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include ../inc/Channel.hpp

Channel(const User &exec, const std::string &name) {
	if (checkServerName(name) == false)
		throw (std::runtime_error());
	this.name = name;
	Member creator = {exec, true};
	this.membs.push_back = creator;
	this.topic = "";
	this.maxmeb = 0;
	this.modes = 0;
	this.passwd = "";
}

// --------------------
// getters and printers
const std::string &getName(void) const {return name;}
const std::string &getTopic(void) const {return topic;}
unsigned int &getMaxMemb(void) const {return maxmeb};
unsigned short &getModes(void) const {return modes};
// ---------------------
// command functions
bool	join(const User &exec) {
	if (this.hasMode())
}

bool	kick(const Member &exec, const Member &target);
bool	message(const Member &exec);

bool	setTopic(const Member &exec, const std::string &topic) {
	//	attempt to execute orders trough getters and setters
	//	write respondes to the buffers
}

//------------------------------------------------

bool	mode(const Member &exec, const short modes);

void	setMode(unsigned char mode){
	std::string reply; 
	if (exec.isoperator == false){
		//send numeric reply
	}
	else {
		this.mode = mode;
		// send non numeric
	}
}

void	unsetMode(unsigned char mode) {  }

void	hasMode(const Member &exec, unsigned int mode) const {}
// ----------------------------------------------
void	modeCommand()
{
	//	parse the string
	//	split by spaces
	//	check param number
	//		if incorrect send num reply
	//	
	//	attempt to execute orders trough getters and setters
	//	write respondes to the buffers
}
