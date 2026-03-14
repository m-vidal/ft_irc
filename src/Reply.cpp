/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 14:24:18 by atambo            #+#    #+#             */
/*   Updated: 2026/03/14 14:47:31 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

void Server::initReplies()
{
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

    // Nickname & Registration Errors
    _replyMessages[ERR_NONICKNAMEGIVEN] = "No nickname given";
    _replyMessages[ERR_ERRONEUSNICKNAME] = "Erroneous nickname";
    _replyMessages[ERR_NICKNAMEINUSE] = "Nickname is already in use";
    _replyMessages[ERR_NOTREGISTERED] = "You have not registered";
    _replyMessages[ERR_ALREADYREGISTERED] = "Unauthorized command (already registered)";
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
std::string Server::formatMessage(const User &source, const std::string &command, const std::string &params)
{
    std::string msg = ":" + source.getPrefix() + " " + command;
    if (!params.empty())
        msg += " " + params;
    return msg;
}

void Server::sendMsg(int fd, const User &source, const std::string &command, const std::string &params)
{
    std::string msg = formatMessage(source, command, params);
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

void Server::checkRegistration(int fd)
{
    if (_users[fd].isAuthenticated() == true)
        return;
    if ((_users[fd].checkIsPassAccepted() == true) && (_users[fd].checkIsUserSet() == true) && (_users[fd].checkIsNickSet() == true))
    {
        _users[fd].setIsAuthenticated();
        std::cout << "User: " << _users[fd].getNick() << " is authenticated." << std::endl;
        sendNumeric(fd, RPL_WELCOME, "Welcome to " + _serverName);
        sendNumeric(fd, RPL_YOURHOST, "Your host is " + _serverName + ", running version 1.0");
        sendNumeric(fd, RPL_CREATED, "This " + _serverName + " was created " + timeToStr(_creationTime));
        sendNumeric(fd, RPL_MYINFO, _serverName + " 1.0 ~NA +" + mode_chars);
        incUsers();
    }
}
