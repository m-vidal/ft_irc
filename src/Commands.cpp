/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 14:10:42 by atambo            #+#    #+#             */
/*   Updated: 2026/03/07 03:33:07 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

// commands
void Server::pass(int fd, std::vector<std::string> &params, std::string trailing)
{
    (void)trailing;
    if (_users[fd].isAuthenticated() == true)
    {
        ircReply(fd, ERR_ALREADYREGISTERED, "PASS", "User already registered!");
        return;
    }
    if (params.size() == 0)
    {
        ircReply(fd, ERR_NEEDMOREPARAMS, "PASS", "Not enough parameters");
        return;
    }
    if (params[0] != _password)
    {
        ircReply(fd, ERR_PASSWDMISMATCH, "PASS", "Wrong password!");
        return;
    }
    _users[fd].setPassAccepted(); // sets passAccepted to true
}
void Server::nick(int fd, std::vector<std::string> &params, std::string trailing)
{
    (void)trailing;
    if (params.empty())
    {
        ircReply(fd, ERR_NONICKNAMEGIVEN, "NICK", "No nickname given");
        return;
    }
    const std::string &newNick = params[0];
    if (std::isdigit(newNick[0]) ||
        newNick.find_first_of(" ,*!@:#\n") != std::string::npos)
    {
        ircReply(fd, ERR_ERRONEUSNICKNAME, "NICK", "Erroneous nickname");
        return;
    }
    User *existing = findUserByNick(newNick);
    if (existing && existing->getFd() != fd)
    {
        return ircReply(fd, ERR_NICKNAMEINUSE, newNick, "Nickname is already in use");
    }
    std::string oldNick = _users[fd].getNick();
    bool wasAuthenticated = _users[fd].isAuthenticated();
    _users[fd].setNick(newNick);
    if (wasAuthenticated)
    {
        std::string message = ":" +
                              oldNick + "!" +
                              _users[fd].getUsername() + "@" +
                              _users[fd].getHostname() +
                              " NICK :" + newNick + "\r\n";
        ircReply(fd, message);

        std::set<int> notified;
        notified.insert(fd);   // seed with sender so self isn't double-notified
        ircReply(fd, message); // send to self explicitly

        for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
        {
            if (it->second.isMember(_users[fd]))
            {
                // broadcastToChannel(it->second, message, notified);
                sendToMembers(it->second, message, notified);
            }
        }
    }
    checkRegistration(fd);
}

void Server::user(int fd, std::vector<std::string> &params, std::string trailing)
{
    _users[fd].setUser(params[0]);
    _users[fd].setHostname(params[1]);
    _users[fd].setRealname(trailing);
    checkRegistration(fd);
}
void Server::ping(int fd, std::vector<std::string> &params, std::string trailing)
{
    (void)trailing;
    (void)params;
    ircReply(fd, ":ircserv PONG ircserv :ircserv");
}

void Server::join(int fd, std::vector<std::string> &params, std::string trailing)
{
    (void)trailing;
    // Fix #2: check params is non-empty before any access
    if (params[0][0] != '#')
    {
        ircReply(fd, ERR_NOSUCHCHANNEL, "JOIN", "No such channel!");
        return;
    }
    std::map<std::string, Channel>::iterator it = _channels.find(params[0]);
    // Fix #1: use find() instead of operator[] to avoid inserting a default Channel
    if (it != _channels.end() && it->second.isMember(_users[fd]))
        return;
    if (it == _channels.end()) // channel dosent exits, create channel
    {
        _channels.insert(std::make_pair(params[0], Channel(params[0])));
        std::cout << "Channel " << params[0] << " created." << std::endl;
        _channels[params[0]].addOperator(_users[fd]);
        sendUserList(_channels[params[0]], fd);
    }
    else // channel exists add user to channel
    {
        _channels[params[0]].addMember(_users[fd]);
        sendUserList(_channels[params[0]], fd);
    }
}

void Server::part(int fd, std::vector<std::string> &params, std::string trailing)
{

    Channel &channel = _channels[params[0]];
    std::string channelName = channel.getName();

    if (_channels.find(channelName) == _channels.end())
    {
        ircReply(fd, ERR_NOSUCHCHANNEL, channelName, "No such channel!");
        return;
    }
    if (!channel.isMember(fd))
    {
        ircReply(fd, ERR_NOTONCHANNEL, channelName, "You're not on that channel.");
        return;
    }
    User &user = _users[fd];
    std::string message = ":" + user.getNick() +
                          "!" + user.getUsername() +
                          "@" + user.getHostname() +
                          " PART " + channelName;

    if (trailing.empty() == false)
    {
        message += " :" + trailing;
    }
    std::set<int> notified;
    // broadcastToChannel(channel, message, notified);
    sendToMembers(channel, message, notified);
    channel.removeMember(fd);
    if (channel.getMemberCount() == 0)
        _channels.erase(channelName);
}

void Server::mode(int fd, std::vector<std::string> &params, std::string trailing)
{
    (void)trailing;
    if (params.size() < 1)
        throw std::runtime_error("error in mode command");
    if (params[0][0] == '#')
        mode_channel(fd, params);
}

void Server::mode_channel(int fd, std::vector<std::string> &params)
{
    std::string name = params[0];
    std::map<std::string, Channel>::iterator it = _channels.find(name);
    if (it == _channels.end())
        return ircReply(fd, ERR_NOSUCHCHANNEL, "MODE", "No such channel");
    Channel chan = it->second;
    if (!chan.isMember(fd))
        return ircReply(fd, ERR_NOTONCHANNEL, "MODE", "You're not on that channel.");
    if (params.size() == 1)
        return ircReply(fd, RPL_CHANNELMODEIS, "MODE", chan.getModeStr());
    if (chan.isOperator(fd))
        chan.applyModeString(params[1]);
    else
        return ircReply(fd, ERR_CHANOPRIVSNEEDED, "MODE", "You're not channel operator");
}