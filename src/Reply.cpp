/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 14:24:18 by atambo            #+#    #+#             */
/*   Updated: 2026/03/11 13:30:30 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

void Server::ircReply(const Channel &channel, int fd, int code, const std::string &command, const std::string &trailing)
{
    std::stringstream ss;

    ss << std::setw(3) << std::setfill('0') << code;
    std::string reply = ":ircserv " + ss.str() + " " + getUserNick(fd) + " " + command + " :" + trailing + "\r\n";
    sendToChannel(channel, reply);
}

void Server::ircReply(int fd, int code, const std::string &command, const std::string &trailing)
{
    std::stringstream ss;

    ss << std::setw(3) << std::setfill('0') << code;
    std::string reply = ":ircserv " + ss.str() + " " + getUserNick(fd) + " " + command + " :" + trailing + "\r\n";
    send(fd, reply.c_str(), reply.size(), 0);
}
void Server::ircReply(int fd, const std::string &msg)
{
    std::string reply = msg + "\r\n";
    send(fd, reply.c_str(), reply.size(), 0);
}
void Server::ircReply(int fd, const std::string &command, const std::string &trailing)
{
    std::map<int, User>::const_iterator it = _users.find(fd);

    if (it == _users.end())
    {
        return; // Or handle error
    }

    const User &user = it->second;

    std::string reply = ":" + user.getNick() +
                        "!" + user.getUsername() +
                        "@" + user.getHostname() +
                        " " + command + " " + trailing + "\r\n";
    std::cout << reply << std::endl;

    send(fd, reply.c_str(), reply.size(), 0);
}

void Server::sendUserList(const Channel &channel, const int &fd)
{
    std::stringstream ss;
    std::stringstream ss1;

    User &user = _users[fd];
    std::string message = ":" + user.getNick() +
                          "!" + user.getUsername() +
                          "@" + user.getHostname() +
                          " JOIN " + channel.getName();
    std::set<int> notified;
    notified.insert(fd);
    // broadcastToChannel(channel, message, notified);
    sendToChannel(channel, message, notified);
    ircReply(fd, "JOIN", channel.getName());

    std::string mode_msg = " MODE " + channel.getName() + channel.getModeStr();
    ircReply(fd, RPL_CHANNELMODEIS, mode_msg, "");

    ss << std::setw(3) << std::setfill('0') << RPL_NAMREPLY;
    ircReply(fd,
             ":ircserv " + ss.str() + " " + getUserNick(fd) + " = " + channel.getName() + " :" + channel.getMemberNickList());
    ss1 << std::setw(3) << std::setfill('0') << RPL_ENDOFNAMES;
    ircReply(fd, ":ircserv " + ss1.str() + " " + getUserNick(fd) + " " + channel.getName() + " :End of /NAMES list");
}

void Server::checkRegistration(int fd)
{
    if (_users[fd].isAuthenticated() == true)
        return;
    if ((_users[fd].checkIsPassAccepted() == true) && (_users[fd].checkIsUserSet() == true) && (_users[fd].checkIsNickSet() == true))
    {
        _users[fd].setIsAuthenticated();
        std::cout << "User: " << _users[fd].getNick() << " is authenticated." << std::endl;
        ircReply(fd, RPL_WELCOME, "", "Welcome to ircserv.");
        ircReply(fd, RPL_YOURHOST, "", "Your host is ircserv, running version 1.0"); // make version and date macros.
        ircReply(fd, RPL_CREATED, "", "This server was created <date>.");
        ircReply(fd, RPL_MYINFO, "", "ircserv 1.0 <user modes> <chan modes> ");
        incUsers();
    }
}