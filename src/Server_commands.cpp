/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_commands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 14:10:42 by atambo            #+#    #+#             */
/*   Updated: 2026/03/12 15:05:55 by atambo           ###   ########.fr       */
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
    _commands["PING"] = Command(&Server::ping, 1, false);
    _commands["JOIN"] = Command(&Server::join, 1, false);
    _commands["PART"] = Command(&Server::part, 1, false);
    _commands["MODE"] = Command(&Server::mode, 1, false);
    _commands["INVITE"] = Command(&Server::invite, 0, false);
    _commands["TOPIC"] = Command(&Server::topic, 1, false);
}
// commands
void Server::pass(int fd, std::vector<std::string> &params, std::string trailing)
{
    (void)trailing;
    if (_users[fd].isAuthenticated() == true)
    {
        sendNumeric(fd, ERR_ALREADYREGISTERED, "PASS", "User already registered!");
        return;
    }
    if (params.size() == 0)
    {
        sendNumeric(fd, ERR_NEEDMOREPARAMS, "INVITE", "Not enough parameters");
        return;
    }
    if (params[0] != _password)
    {
        sendNumeric(fd, ERR_PASSWDMISMATCH, "PASS", "Wrong password!");
        return;
    }
    _users[fd].setPassAccepted(); // sets passAccepted to true
}

void Server::nick(int fd, std::vector<std::string> &params, std::string trailing)
{
    // If your parser puts the nick in trailing when no colon is used,
    // or if it's the only param, we check both.
    std::string newNick = (params.empty()) ? trailing : params[0];

    if (newNick.empty())
    {
        sendNumeric(fd, ERR_NONICKNAMEGIVEN, "", "No nickname given");
        return;
    }

    // IRC Nick Rules: No leading digits, no forbidden chars
    if (std::isdigit(newNick[0]) || newNick.find_first_of(" ,*!@:#\n") != std::string::npos)
    {
        sendNumeric(fd, ERR_ERRONEUSNICKNAME, newNick, "Erroneous nickname");
        return;
    }

    // Case-insensitive check (assuming findUserByNick handles it)
    User *existing = findUserByNick(newNick);
    if (existing && existing->getFd() != fd)
    {
        sendNumeric(fd, ERR_NICKNAMEINUSE, newNick, "Nickname is already in use");
        return;
    }

    User &user = _users[fd];
    std::string oldPrefix = user.getPrefix(); // Capture prefix BEFORE changing nick
    bool wasAuthenticated = user.isAuthenticated();

    user.setNick(newNick);

    if (wasAuthenticated)
    {
        // Format: :oldnick!user@host NICK :newnick
        // We use a raw sender here because NICK isn't a numeric reply
        std::string notifyMsg = ":" + oldPrefix + " NICK :" + newNick + "\r\n";

        // 1. Send to self
        sendToClient(fd, notifyMsg);

        // 2. Notify everyone who shares a channel with the user
        std::set<int> notified;
        notified.insert(fd); // Don't send to self again via broadcast

        std::map<std::string, Channel>::iterator it;
        for (it = _channels.begin(); it != _channels.end(); ++it)
        {
            if (it->second.isMember(user))
            {
                sendToChannel(it->second, notifyMsg, notified);
            }
        }
    }

    // Check if this NICK command completes their registration
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
    // The token is usually in params[0] or trailing (depending on the client)
    std::string token = params.empty() ? trailing : params[0];
    std::string pongMsg = ":" + _serverName + " PONG " + _serverName + " :" + token + "\r\n";
    sendToClient(fd, pongMsg);
}

void Server::join(int fd, std::vector<std::string> &params, std::string trailing)
{
    (void)trailing;
    // Fix #2: check params is non-empty before any access
    User &user = _users[fd];
    std::string channel_name = params[0];
    if (!valid_channel_name(channel_name))
        return sendNumeric(fd, ERR_BADCHANMASK, channel_name, "Bad Channel Mask");
    std::map<std::string, Channel>::iterator it = _channels.find(channel_name);
    if (it != _channels.end() && it->second.isMember(user))
        return;
    if (user.getChannelCount() > MAX_JOINED_CHAN)
        return sendNumeric(fd, ERR_TOOMANYCHANNELS, channel_name, "You have joined too many channels");
    if (it == _channels.end()) // channel dosent exits, create channel
    {
        _channels.insert(std::make_pair(channel_name, Channel(channel_name)));
        std::cout << "Channel " << channel_name << " created." << std::endl;
        std::map<std::string, Channel>::iterator it = _channels.find(channel_name);
        if (it != _channels.end())
            it->second.addOperator(user);
    }
    else if (it != _channels.end() && !(it->second.isMember(user)))
    {
        Channel &channel = it->second;
        if (channel.hasMode('k'))
        {
            std::string key = "";
            if (params.size() > 1)
                key = params[1];
            else
                return sendNumeric(fd, ERR_INVALIDMODEPARAM, "JOIN", "channel requires a key");

            if (!channel.verifyKey(key))
                return sendNumeric(fd, ERR_BADCHANNELKEY, "JOIN", "Cannot join channel (+k) - bad key");
        }
        if (channel.hasMode('i') && !channel.isInvited(user.getNick()))
            return sendNumeric(fd, ERR_INVITEONLYCHAN, "JOIN", "Cannot join channel (+i) - you must be invited");

        it->second.addMember(user);
    }
    if (it != _channels.end())
        sendUserList(it->first, fd);
}

void Server::part(int fd, std::vector<std::string> &params, std::string trailing)
{
    std::string channel_name = params[0];
    std::map<std::string, Channel>::iterator it = _channels.find(channel_name);
    if (it == _channels.end())
        return sendNumeric(fd, ERR_NOSUCHCHANNEL, channel_name, "No such channel!");

    Channel channel = it->second;
    if (!channel.isMember(fd))
        return sendNumeric(fd, ERR_NOTONCHANNEL, channel_name, "You're not on that channel.");

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
            std::string username = it->first;
            std::stringstream ss;
            ss << it->second;
            std::string timeStr = ss.str();
            sendNumeric(fd, RPL_INVITELIST, username, timeStr);
        }
        return sendNumeric(fd, RPL_ENDOFINVITELIST, username, "End of /INVITE list");
    }
    else if (params.size() == 2)
    {
        User *target = findUserByNick(params[0]);
        if (!target)
            return sendNumeric(fd, ERR_NOSUCHNICK, params[0], "User doesn't exist");

        std::map<std::string, Channel>::iterator it = _channels.find(params[1]);
        if (it == _channels.end())
            return sendNumeric(fd, ERR_NOSUCHCHANNEL, params[1], "No such channel!");

        Channel &channel = it->second;

        if (!channel.isMember(fd))
            return sendNumeric(fd, ERR_NOTONCHANNEL, params[1], "You're not on that channel.");

        if (channel.hasMode('i') && !channel.isOperator(fd))
            return sendNumeric(fd, ERR_CHANOPRIVSNEEDED, params[1], "You're not channel operator");

        if (channel.isMember(target->getFd()))
            return sendNumeric(fd, ERR_USERONCHANNEL, target->getNick() + " " + params[1], "is already on channel");

        // 1. Add to the channel's internal invite list
        channel.addInvite(*target);

        // 2. Notify the INVITER (Standard Numeric)
        sendNumeric(fd, RPL_INVITING, target->getNick(), channel.getName());

        // 3. Notify the INVITED User (Raw Command)
        // Format: :InviterNick!InviterUser@InviterHost INVITE TargetNick :ChannelName
        std::string inviteMsg = ":" + _users[fd].getPrefix() + " INVITE " + target->getNick() + " :" + channel.getName() + "\r\n";
        return sendToClient(target->getFd(), inviteMsg);
    }
    return sendNumeric(fd, ERR_NEEDMOREPARAMS, "INVITE", "Not enough parameters");
}

void Server::topic(int fd, std::vector<std::string> &params, std::string trailing)
{
    if (params.size() < 1)
        throw std::runtime_error("error in Server::topic");
    std::string channel_name = params[0];
    std::map<std::string, Channel>::iterator it = _channels.find(channel_name);
    if (it == _channels.end())
        return sendNumeric(fd, ERR_NOSUCHCHANNEL, channel_name, "No such channel!");
    Channel &channel = it->second;
    if (!channel.isMember(fd))
        return sendNumeric(fd, ERR_NOTONCHANNEL, channel_name, "You're not on that channel.");

    if (params.size() == 1)
    {
        if (channel.getTopic().content.empty())
            return sendNumeric(fd, RPL_NOTOPIC, channel_name, "No topic is set");
        sendNumeric(fd, RPL_TOPIC, channel_name, channel.getTopic().content);
        std::string time_str = timeToStr(channel.getTopic().creationTime);
        return sendNumeric(fd, RPL_TOPICWHOTIME, channel_name, channel.getTopic().setBy + " " + time_str);
    }
    if (!channel.isOperator(fd) && channel.hasMode('t'))
        return sendNumeric(fd, ERR_CHANOPRIVSNEEDED, channel_name, "You're not channel operator");
    channel.setTopic(trailing, _users[fd].getNick());
}
