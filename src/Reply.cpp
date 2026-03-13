/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 14:24:18 by atambo            #+#    #+#             */
/*   Updated: 2026/03/13 17:09:27 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

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
    ss << " :" << getNumericMsg(code) << "\r\n";

    return ss.str();
}

// For Commands (e.g., :nick!user@host JOIN #channel)
std::string Server::formatMessage(const User &source, const std::string &command, const std::string &params)
{
    std::string msg = ":" + source.getPrefix() + " " + command;
    if (!params.empty())
        msg += " " + params;
    return msg + "\r\n";
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
        sendNumeric(fd, RPL_WELCOME, "Welcome to ircserv.");
        sendNumeric(fd, RPL_YOURHOST, "Your host is ircserv, running version 1.0"); // make version and date macros.
        sendNumeric(fd, RPL_CREATED, "This server was created <date>.");
        sendNumeric(fd, RPL_MYINFO, "ircserv 1.0 <user modes> <chan modes> ");
        incUsers();
    }
}
void Server::sendUserList(const Channel &channel, int fd)
{
    User &user = _users[fd];
    std::string nick = user.getNick();
    std::string chanName = channel.getName();

    // 1. Tell everyone (including joining user) about the JOIN
    std::string joinMsg = formatMessage(user, "JOIN", chanName);
    sendToChannel(channel, joinMsg, -1);

    // 2. Send Topic (if exists) - Good addition here!
    if (!channel.getTopic().content.empty())
    {
        sendToClient(fd, formatNumeric(RPL_TOPIC, nick, chanName + channel.getTopic().content));
    }

    // 3. Send Name List (353) and End of Names (366)
    sendToClient(fd, formatNumeric(RPL_NAMREPLY, nick, "= " + chanName + channel.getMemberNickList()));
    sendToClient(fd, formatNumeric(RPL_ENDOFNAMES, nick, chanName + " End of /NAMES list"));
}