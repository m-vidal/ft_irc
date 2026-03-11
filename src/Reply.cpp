/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 14:24:18 by atambo            #+#    #+#             */
/*   Updated: 2026/03/11 17:21:15 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

// For Numerics (e.g., :ircserv 001 target :Welcome)
std::string Server::formatNumeric(int code, const std::string &nick, const std::string &params, const std::string &trailing)
{
    std::stringstream ss;
    ss << ":" << _serverName << " " << std::setw(3) << std::setfill('0') << code << " " << nick;
    if (!params.empty())
        ss << " " << params;
    if (!trailing.empty())
        ss << " :" << trailing;
    return ss.str() + "\r\n";
}

// For Commands (e.g., :nick!user@host JOIN #channel)
std::string Server::formatMessage(const User &source, const std::string &command, const std::string &params, const std::string &trailing)
{
    std::string msg = ":" + source.getPrefix() + " " + command;
    if (!params.empty())
        msg += " " + params;
    if (!trailing.empty())
        msg += " :" + trailing;
    return msg + "\r\n";
}

void Server::sendUserList(const Channel &channel, int fd)
{
    User &user = _users[fd];
    std::string nick = user.getNick();
    std::string chanName = channel.getName();

    // 1. Tell everyone (including joining user) about the JOIN
    std::string joinMsg = formatMessage(user, "JOIN", chanName, "");
    sendToChannel(channel, joinMsg, -1);

    // 2. Send Topic (if exists) - Good addition here!
    if (!channel.getTopic().content.empty())
    {
        sendToClient(fd, formatNumeric(RPL_TOPIC, nick, chanName, channel.getTopic().content));
    }

    // 3. Send Name List (353) and End of Names (366)
    sendToClient(fd, formatNumeric(RPL_NAMREPLY, nick, "= " + chanName, channel.getMemberNickList()));
    sendToClient(fd, formatNumeric(RPL_ENDOFNAMES, nick, chanName, "End of /NAMES list"));
}

void Server::sendNumeric(int fd, int code, const std::string &params, const std::string &trailing)
{
    std::string nick = _users[fd].getNick();
    if (nick.empty())
        nick = "*"; // RFC standard for users not yet fully registered

    std::string msg = formatNumeric(code, nick, params, trailing);
    send(fd, msg.c_str(), msg.size(), 0);
}

void Server::sendNumeric(Channel &channel, int fd, int code, const std::string &params, const std::string &trailing)
{
    // 1. Get the nick of the user who triggered this (or "*" if not set)
    std::string nick = _users[fd].getNick();
    if (nick.empty())
        nick = "*";

    // 2. Generate the formatted numeric string using your helper
    // Format: :ircserv <CODE> <target_nick> <params> :<trailing>
    std::string msg = formatNumeric(code, nick, params, trailing);

    // 3. Broadcast to the channel, skipping the person who triggered it
    this->sendToChannel(channel, msg, fd);
}