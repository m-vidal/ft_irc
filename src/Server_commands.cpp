/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_commands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 14:10:42 by atambo            #+#    #+#             */
/*   Updated: 2026/03/11 13:03:10 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

void Server::setknowncommands(void)
{
    // _commands["PRIVMSG"] = &Server::msg;
    _commands["PASS"] = Command(&Server::pass, 1, false);
    _commands["NICK"] = Command(&Server::nick, 0, false);
    _commands["USER"] = Command(&Server::user, 4, false);
    _commands["PING"] = Command(&Server::ping, 0, false);
    _commands["JOIN"] = Command(&Server::join, 1, false);
    _commands["PART"] = Command(&Server::part, 1, false);
    _commands["MODE"] = Command(&Server::mode, 1, false);
    _commands["INVITE"] = Command(&Server::invite, 0, false);
}
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
                sendToChannel(it->second, message, notified);
            }
        }
    }
    checkRegistration(fd);
}

void Server::user(int fd, std::vector<std::string> &params, std::string trailing)
{
    _users[fd].setUser(params[0]);
    _users[fd].setRealname(trailing);
    checkRegistration(fd);
}
void Server::ping(int fd, std::vector<std::string> &params, std::string trailing)
{
    (void)trailing;
    (void)params;
    return ircReply(fd, ":ircserv PONG ircserv :ircserv");
}

void Server::join(int fd, std::vector<std::string> &params, std::string trailing)
{
    (void)trailing;
    // Fix #2: check params is non-empty before any access
    if (!valid_channel_name(params[0]))
        return ircReply(fd, ERR_BADCHANMASK, params[0], "Bad Channel Mask");

    std::map<std::string, Channel>::iterator it = _channels.find(params[0]);
    if (it != _channels.end() && it->second.isMember(_users[fd]))
        return;
    std::string name = params[0];
    if (it == _channels.end()) // channel dosent exits, create channel
    {
        _channels.insert(std::make_pair(name, Channel(name)));
        std::cout << "Channel " << name << " created." << std::endl;
        _channels[name].addOperator(_users[fd]);
    }
    else if (it != _channels.end() && !(it->second.isMember(_users[fd])))
    {
        Channel &channel = it->second;
        if (channel.hasMode('k'))
        {
            std::string key = "";
            if (params.size() > 1)
                key = params[1];
            else
                return ircReply(fd, ERR_INVALIDMODEPARAM, "JOIN", "channel requires a key");

            if (!channel.verifyKey(key))
                return ircReply(fd, ERR_BADCHANNELKEY, "JOIN", "Cannot join channel (+k) - bad key");
        }
        if (channel.hasMode('i') && !channel.isInvited(_users[fd].getNick()))
            return ircReply(fd, ERR_INVITEONLYCHAN, "JOIN", "Cannot join channel (+i) - you must be invited");

        _channels[name].addMember(_users[fd]);
    }
    sendUserList(_channels[name], fd);
}

void Server::part(int fd, std::vector<std::string> &params, std::string trailing)
{
    std::string channel_name = params[0];
    std::map<std::string, Channel>::iterator it = _channels.find(channel_name);
    if (it == _channels.end())
        return ircReply(fd, ERR_NOSUCHCHANNEL, channel_name, "No such channel!");

    Channel channel = it->second;
    if (!channel.isMember(fd))
        return ircReply(fd, ERR_NOTONCHANNEL, channel_name, "You're not on that channel.");

    User &user = _users[fd];
    std::string message = ":" + user.getNick() +
                          "!" + user.getUsername() +
                          "@" + user.getHostname() +
                          " PART " + channel_name;

    if (trailing.empty() == false)
    {
        message += " :" + trailing;
    }
    std::set<int> notified;
    // broadcastToChannel(channel, message, notified);
    sendToChannel(channel, message, notified);
    channel.removeMember(fd);
    if (channel.getMemberCount() == 0)
        _channels.erase(channel_name);
}

void Server::invite(int fd, std::vector<std::string> &params, std::string trailing)
{
    (void)trailing;
    std::string username = _users[fd].getUsername();
    User user = _users[fd];
    if (params.size() == 0)
    {
        const std::map<std::string, time_t> &invites = user.getInvites();
        std::map<std::string, time_t>::const_iterator it;
        for (it = invites.begin(); it != invites.end(); ++it)
        {
            std::string channelName = it->first;
            std::stringstream ss;
            ss << it->second;
            std::string timeStr = ss.str();
            ircReply(fd, RPL_INVITELIST, channelName, timeStr);
        }
        ircReply(fd, RPL_ENDOFINVITELIST, username, "End of /INVITE list");
    }
    else if (params.size() == 2)
    {
        User *target = findUserByNick(params[0]);
        if (!target)
            return ircReply(fd, ERR_NOSUCHNICK, params[0], "User doesnt exist");
        std::map<std::string, Channel>::iterator it = _channels.find(params[1]);
        std::cout << "channel name = " << params[1] << "\n";
        if (it == _channels.end())
            return ircReply(fd, ERR_NOSUCHCHANNEL, params[1], "No such channel!");
        Channel &channel = it->second;
        if (!channel.isMember(fd))
            return ircReply(fd, ERR_NOTONCHANNEL, params[1], "You're not on that channel.");
        if (!channel.isOperator(fd))
            return ircReply(fd, ERR_CHANOPRIVSNEEDED, params[1], "You're not channel operator");
        channel.addInvite(*target);
        return ircReply(fd, RPL_INVITING, target->getNick(), channel.getName());
    }
    else
        return ircReply(fd, ERR_NEEDMOREPARAMS, "INVITE", "Need more params nigga");
}
