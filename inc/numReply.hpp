/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numReply.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 15:09:27 by atambo            #+#    #+#             */
/*   Updated: 2026/02/26 18:34:15 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMREPLY_HPP
#define NUMREPLY_HPP

// --- Registration Replies ---
#define RPL_WELCOME "001"
#define RPL_YOURHOST "002"
#define RPL_CREATED "003"
#define RPL_MYINFO "004"

// --- Success Replies (RPL) ---
#define RPL_NOTOPIC "331"
#define RPL_TOPIC "332"
#define RPL_NAMREPLY "353"
#define RPL_ENDOFNAMES "366"

// --- Error Replies (ERR) ---
#define ERR_NOSUCHNICK "401"
#define ERR_NOSUCHCHANNEL "403"
#define ERR_CANNOTSENDTOCHAN "404"
#define ERR_TOOMANYCHANNELS "405"
#define ERR_NONICKNAMEGIVEN "431"
#define ERR_ERRONEUSNICKNAME "432"
#define ERR_NICKNAMEINUSE "433"
#define ERR_USERNOTINCHANNEL "441"
#define ERR_NOTONCHANNEL "442"
#define ERR_USERONCHANNEL "443"
#define ERR_NOTREGISTERED "451"
#define ERR_NEEDMOREPARAMS "461"
#define ERR_ALREADYREGISTRED "462"
#define ERR_PASSWDMISMATCH "464"
#define ERR_CHANNELISFULL "471"
#define ERR_UNKNOWNMODE "472"
#define ERR_INVITEONLYCHAN "473"
#define ERR_BADCHANNELKEY "475"
#define ERR_CHANOPRIVSNEEDED "482" // not channel operator

#include <string>
// ----------------
#include "Server.hpp"
namespace Reply
{
	/*	This is the generic template for all numerics
		Format: :<server> <numeric> <target> <args> :<message> */
	std::string numeric(
		const Server &server,
		const char *code,
		const std::string &target,
		const std::string &subject,
		const std::string &message);
}
#endif