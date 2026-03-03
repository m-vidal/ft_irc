/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numReply.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 15:09:27 by atambo            #+#    #+#             */
/*   Updated: 2026/03/03 11:08:30 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NUMREPLY_HPP
#define NUMREPLY_HPP

#include <string>

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
#define ERR_UNKNOWNCOMMAND "421"
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
#define ERR_CHANOPRIVSNEEDED "482"

#include <string>
// ----------------
#include "User.hpp"

namespace Reply
{
	/* Events */
	std::string join(const User &u, const std::string &channel);
	std::string privmsg(const User &u, const std::string &target, const std::string &msg);
	std::string nick(const std::string &oldPrefix, const std::string &newNick);
	std::string part(const User &u, const std::string &channel, const std::string &reason);

	/* Handshake */
	std::string welcome(const std::string &srv, const std::string &nick);
	std::string yourhost(const std::string &srv, const std::string &nick);

	/* Info */
	std::string topic(const std::string &srv, const std::string &nick, const std::string &chan, const std::string &topicText);
	std::string namreply(const std::string &srv, const std::string &nick, const std::string &chan, const std::string &users);
	std::string endofnames(const std::string &srv, const std::string &nick, const std::string &chan);

	/* Errors */
	std::string err_nicknameinuse(const std::string &srv, const std::string &nick);
	std::string err_needmoreparams(const std::string &srv, const std::string &nick, const std::string &command);
	std::string err_nosuchchannel(const std::string &srv, const std::string &nick, const std::string &chan);
	std::string err_chanoprivsneeded(const std::string &srv, const std::string &nick, const std::string &chan);
	std::string err_unknowncommand(const std::string &srv, const std::string &nick, const std::string &cmd);
	std::string err_disconnect(const std::string &hostname, const std::string &reason);

}

#endif