/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_commands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 14:10:42 by atambo            #+#    #+#             */
/*   Updated: 2026/03/10 12:50:38 by atambo           ###   ########.fr       */
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
                sendToMembers(it->second, message, notified);
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
    if (params[0][0] != '#')
    {
        // check channel name and send error if bad
        return;
    }
    std::map<std::string, Channel>::iterator it = _channels.find(params[0]);
    // Fix #1: use find() instead of operator[] to avoid inserting a default Channel
    if (it != _channels.end() && it->second.isMember(_users[fd]))
        return;
    std::string name = params[0];
    if (it == _channels.end()) // channel dosent exits, create channel
    {
        _channels.insert(std::make_pair(name, Channel(name)));
        std::cout << "Channel " << name << " created." << std::endl;
        _channels[name].addOperator(_users[fd]);
    }
    else // channel exists add user to channel
    {
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
    sendToMembers(channel, message, notified);
    channel.removeMember(fd);
    if (channel.getMemberCount() == 0)
        _channels.erase(channel_name);
}

void Server::mode(int fd, std::vector<std::string> &params, std::string trailing)
{
    (void)fd;
    (void)params;
    (void)trailing;
    std::string channel_name = params[0];
    std::map<std::string, Channel>::iterator it = _channels.find(channel_name);
    if (it == _channels.end())
        return ircReply(fd, ERR_NOSUCHCHANNEL, channel_name, "No such channel!");

    Channel &channel = (it->second);
    if (!channel.isMember(fd))
        return ircReply(fd, ERR_NOTONCHANNEL, channel_name, "You're not on that channel.");
    if (params.size() == 1)
    {
        ircReply(fd, RPL_CHANNELMODEIS, channel_name, channel.getModeStr());
        return ircReply(fd, RPL_CREATIONTIME, channel_name, channel.getCreationTimeStr());
    }
    if (params.size() == 2)
    {
        if (!channel.isOperator(fd))
            return ircReply(fd, ERR_CHANOPRIVSNEEDED, channel_name, "You're not channel operator");

        applyModeString(fd, params, trailing, channel);
    }
    ircReply(fd, RPL_CHANNELMODEIS, channel_name, channel.getModeStr());
}

void Server::applyModeString(int fd, std::vector<std::string> &params, std::string trailing, Channel &channel)
{
    (void)trailing;
    bool adding = true;

    std::string modes = params[0];
    for (size_t i = 0; i < modes.length(); ++i)
    {
        size_t j = 1;
        char c = modes[i];
        if (c == '+')
        {
            adding = true;
            continue;
        }
        if (c == '-')
        {
            adding = false;
            continue;
        }

        if (adding)
        {
            if (c == 'k')
            {
                if (params.size() <= j)
                    return ircReply(fd, ERR_INVALIDMODEPARAM, channel.getName(), " +k missing key parameter");
                if (params[j].find_first_of(" ") != params[j].npos)
                    return ircReply(fd, ERR_INVALIDMODEPARAM, channel.getName(), " +k '" + params[j] + "' key must not have spaces");
                if (params[j].size() > channel.MAX_KEY_LEN)
                {
                    std::stringstream ss;
                    ss << "+k '" << params[j] << "' key must not exceed "
                       << channel.MAX_KEY_LEN << " characters";

                    return ircReply(fd, ERR_INVALIDMODEPARAM, channel.getName(), ss.str());
                }
                channel.setKey(params[j]);
            }
            channel.setMode(c);
        }
        else
            channel.unsetMode(c);
    }
}
