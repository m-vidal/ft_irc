/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 14:24:18 by atambo            #+#    #+#             */
/*   Updated: 2026/03/14 16:23:41 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

void Server::initReplies()
{
    _replyMessages[ERR_NOTEXTTOSEND] = "No text to send";
    // replies
    _replyMessages[RPL_ENDOFINVITELIST] = "End of /INVITE list";
    _replyMessages[RPL_ENDOFNAMES] = "End of /NAMES list";
    // Channel & Mode Errors
    _replyMessages[ERR_NOSUCHNICK] = "No such nick/channel";
    _replyMessages[ERR_NOSUCHCHANNEL] = "No such channel";
    _replyMessages[ERR_CANNOTSENDTOCHAN] = "Cannot send to channel";
    _replyMessages[ERR_TOOMANYCHANNELS] = "You have joined too many channels";
    _replyMessages[ERR_CHANNELISFULL] = "Cannot join channel (+l)";
    _replyMessages[ERR_INVITEONLYCHAN] = "Cannot join channel (+i)";
    _replyMessages[ERR_BADCHANNELKEY] = "Cannot join channel (+k)";
    _replyMessages[ERR_CHANOPRIVSNEEDED] = "You're not channel operator";
    _replyMessages[ERR_USERNOTINCHANNEL] = "They aren't on that channel";
    _replyMessages[ERR_NOTONCHANNEL] = "You're not on that channel";
    _replyMessages[ERR_USERONCHANNEL] = "is already on channel";
    _replyMessages[ERR_UNKNOWNMODE] = "is unknown mode char to me";
    _replyMessages[ERR_BADCHANMASK] = "Bad Channel Mask";
    _replyMessages[ERR_CANNOTSENDTOCHAN] = "Cannot send to channel";
    // Nickname & Registration Errors
    _replyMessages[ERR_NONICKNAMEGIVEN] = "No nickname given";
    _replyMessages[ERR_ERRONEUSNICKNAME] = "Erroneous nickname";
    _replyMessages[ERR_NICKNAMEINUSE] = "Nickname is already in use";
    _replyMessages[ERR_NOTREGISTERED] = "You have not registered";
    _replyMessages[ERR_ALREADYREGISTERED] = "You are already connected and cannot handshake again";
    _replyMessages[ERR_PASSWDMISMATCH] = "Password incorrect";

    // Misc Errors
    _replyMessages[ERR_UNKNOWNCOMMAND] = "Unknown command";
    _replyMessages[ERR_NEEDMOREPARAMS] = "Not enough parameters";
    _replyMessages[ERR_INVALIDMODEPARAM] = "Invalid mode parameter";
}

// For Numerics (e.g., :ircserv 001 target :Welcome)
std::string Server::formatNumeric(int code, const std::string &nick, const std::string &arg)
{
    std::stringstream ss;
    // Format: :<servername> <3-digit-code> <target_nick>
    ss << ":" << _serverName << " " << std::setw(3) << std::setfill('0') << code << " " << nick;

    // Add the argument (like channel name or nick) if it exists
    if (!arg.empty())
        ss << " " << arg;

    // Append the message from our map
    std::string msg = getNumericMsg(code);
    if (!msg.empty())
        ss << " :" << getNumericMsg(code) << "\r\n";
    return ss.str();
}

// For Commands (e.g., :nick!user@host JOIN #channel)
std::string Server::formatNotice(const User &source, const std::string &command, const std::string &params)
{
    std::string msg = ":" + source.getPrefix() + " " + command;
    if (!params.empty())
        msg += " " + params;
    return msg;
}

void Server::sendNotice(int fd, const User &source, const std::string &command, const std::string &params)
{
    std::string msg = formatNotice(source, command, params);
    sendToClient(fd, msg);
}

void Server::sendNumeric(int fd, int code, const std::string &params)
{
    std::string nick = _users[fd].getNick();
    std::string msg = formatNumeric(code, nick, params);
    sendToClient(fd, msg);
}

void Server::sendNumeric(Channel &channel, int fd, int code, const std::string &params)
{
    // 1. Get the nick of the user who triggered this (or "*" if not set)
    std::string nick = _users[fd].getNick();
    if (nick.empty())
        nick = "*";

    // 2. Generate the formatted numeric string using your helper
    // Format: :ircserv <CODE> <target_nick> <params> :<trailing>
    std::string msg = formatNumeric(code, nick, params);

    // 3. Broadcast to the channel, skipping the person who triggered it
    this->sendToChannel(channel, msg, fd);
}

void Server::sendNumeric(int fd, int code)
{
    sendNumeric(fd, code, "");
}
