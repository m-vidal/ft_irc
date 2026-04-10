/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_commands.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 14:10:42 by atambo            #+#    #+#             */
/*   Updated: 2026/03/29 20:09:26 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

void Server::setknowncommands(void)
{
    // command | min params needed
    _commands["PASS"] = Command(&Server::pass, 1);
    _commands["NICK"] = Command(&Server::nick, 0);
    _commands["USER"] = Command(&Server::user, 4);
    _commands["PING"] = Command(&Server::ping, 1);
    _commands["JOIN"] = Command(&Server::join, 1);
    _commands["PART"] = Command(&Server::part, 1);
    _commands["MODE"] = Command(&Server::mode, 1);
    _commands["INVITE"] = Command(&Server::invite, 0);
    _commands["TOPIC"] = Command(&Server::topic, 1);
    _commands["PRIVMSG"] = Command(&Server::msg, 1);
    _commands["KICK"] = Command(&Server::kick, 2);
    _commands["NOTICE"] = Command(&Server::notice, 0);
    _commands["QUIT"] = Command(&Server::quit, 0);
    _commands["NAMES"] = Command(&Server::names, 0);
}
// commands
void Server::pass(int fd, std::vector<std::string> &params)
{
    if (_users[fd].checkIsPassAccepted() == true)
        return sendNumeric(fd, ERR_ALREADYREGISTERED, "PASS");

    if (params[0] != _password)
        return sendNumeric(fd, ERR_PASSWDMISMATCH, "PASS");

    _users[fd].setPassAccepted(); // sets passAccepted to true
    checkRegistration(fd);
}

void Server::nick(int fd, std::vector<std::string> &params)
{
    // If your parser puts the nick in trailing when no colon is used,
    // or if it's the only param, we check both.
    if (params.empty())
    {
        sendNumeric(fd, ERR_NONICKNAMEGIVEN);
        return;
    }
    std::string newNick = params[0];
    // IRC Nick Rules: No leading digits, no forbidden chars
    if (std::isdigit(newNick[0]) || newNick.find_first_of(" ,*!@:#\n") != std::string::npos)
    {
        sendNumeric(fd, ERR_ERRONEUSNICKNAME, newNick);
        return;
    }
    // Case-insensitive check (assuming findUserByNick handles it)
    if (isNickTaken(newNick, fd))
    {
        sendNumeric(fd, ERR_NICKNAMEINUSE, newNick);
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
    std::cout << "nickname set\n";
    checkRegistration(fd);
}

void Server::user(int fd, std::vector<std::string> &params)
{
    _users[fd].setUser(params[0]);
    _users[fd].setRealname(params[1]);
    std::cout << "username set\n";
    checkRegistration(fd);
}

void Server::ping(int fd, std::vector<std::string> &params)
{
    // The token is usually in params[0] or trailing (depending on the client)
    std::string token = params[0];
    std::string pongMsg = ":" + _serverName + " PONG " + _serverName + " :" + token + "\r\n";
    sendToClient(fd, pongMsg);
}

void Server::join(int fd, std::vector<std::string> &params)
{
    // Fix #2: check params is non-empty before any access
    User &user = _users[fd];
    std::string channel_name = params[0];
    if (!valid_channel_name(channel_name))
        return sendNumeric(fd, ERR_BADCHANMASK, channel_name);
    //------------------------------------------------------
    std::map<std::string, Channel>::iterator it;
    it = _channels.find(channel_name);
    if (it != _channels.end() && it->second.isMember(user))
        return;
    if (user.getChannelCount() + 1 > MAX_JOINED_CHAN)
        return sendNumeric(fd, ERR_TOOMANYCHANNELS, channel_name);
    //------------------------------------------------------
    if (it == _channels.end()) // channel dosent exits, create channel
    {
        _channels.insert(std::make_pair(channel_name, Channel(channel_name, mode_chars)));
        std::cout << "Channel " << channel_name << " created." << std::endl;
        std::map<std::string, Channel>::iterator it = _channels.find(channel_name);
        if (it != _channels.end())
        {
            user.addChannel(channel_name);
            it->second.addMember(user);
            it->second.setOperator(user, true);
        }
    }
    else if (it != _channels.end() && !(it->second.isMember(user))) // if exist try to join
    {
        Channel &channel = it->second;
        if (channel.hasMode('k'))
        {
            std::string key = "";
            if (params.size() > 1)
                key = params[1];
            else
                return sendNumeric(fd, ERR_INVALIDMODEPARAM, "JOIN");

            if (!channel.verifyKey(key))
                return sendNumeric(fd, ERR_BADCHANNELKEY, "JOIN");
        }
        if (channel.hasMode('i') && !channel.isInvited(user.getNick()))
            return sendNumeric(fd, ERR_INVITEONLYCHAN, "JOIN");
        if (channel.hasMode('l') && channel.getMemberCount() >= channel.getLimit())
            return sendNumeric(fd, ERR_CHANNELISFULL, "JOIN");
        user.addChannel(channel_name);
        it->second.addMember(user);
    }
    //------------------------------------------------------
    it = _channels.find(channel_name);
    if (it != _channels.end())
    {
        Channel &channel = it->second;
        std::string msg = formatNotice(user, "JOIN", channel_name);
        sendToChannel(channel, msg, 0);
        topic(fd, params);
        mode(fd, params);
        names(fd, params);
    }
}

void Server::part(int fd, std::vector<std::string> &params)
{
    if (params.empty())
        return sendNumeric(fd, ERR_NEEDMOREPARAMS, "PART");
    std::string channel_name = params[0];
    std::map<std::string, Channel>::iterator it = _channels.find(channel_name);
    if (it == _channels.end())
        return sendNumeric(fd, ERR_NOSUCHCHANNEL, channel_name);
    Channel &channel = it->second; 
    if (!channel.isMember(fd))
        return sendNumeric(fd, ERR_NOTONCHANNEL, channel_name);
    User &user = _users.at(fd);
    std::string message = ":" + user.getPrefix() + " PART " + channel_name;
    if (params.size() > 1)
        message += " :" + params[1];
    sendToChannel(channel, message, -1); 
    channel.removeMember(fd);
    user.removeChannel(channel_name);
    if (channel.getMemberCount() == 0)
    {
        _channels.erase(it);
        std::cout << "Channel " << channel_name << " has been erased (empty)." << std::endl;
    }
}

void Server::invite(int fd, std::vector<std::string> &params)
{
    std::string username = _users[fd].getUsername();
    User &user = _users[fd];
    if (params.size() == 0)
    {
        const std::map<std::string, time_t> &invites = user.getInvites();
        std::map<std::string, time_t>::const_iterator it;
        for (it = invites.begin(); it != invites.end(); ++it)
        {
            std::string channel_name = it->first;
            std::stringstream ss;
            ss << it->second;
            std::string timeStr = ss.str();
            sendNumeric(fd, RPL_INVITELIST, channel_name);
        }
        return sendNumeric(fd, RPL_ENDOFINVITELIST);
    }
    else if (params.size() == 2)
    {
        User *target = findUserByNick(params[0]);
        if (!target)
            return sendNumeric(fd, ERR_NOSUCHNICK, params[0]);
        std::map<std::string, Channel>::iterator it = _channels.find(params[1]);
        if (it == _channels.end())
            return sendNumeric(fd, ERR_NOSUCHCHANNEL, params[1]);
        Channel &channel = it->second;
        if (!channel.isMember(fd))
            return sendNumeric(fd, ERR_NOTONCHANNEL, params[1]);
        if (channel.hasMode('i') && !channel.isOperator(fd))
            return sendNumeric(fd, ERR_CHANOPRIVSNEEDED, params[1]);
        if (channel.isMember(target->getFd()))
            return sendNumeric(fd, ERR_USERONCHANNEL, target->getNick() + " " + params[1]);
        // 1. Add to the channel's internal invite list
        channel.addInvite(*target);
        // 2. Notify the INVITER (Standard Numeric)
        sendNumeric(fd, RPL_INVITING, target->getNick() + " " + channel.getName());
        // 3. Notify the INVITED User (Raw Command)
        // Format: :InviterNick!InviterUser@InviterHost INVITE TargetNick :ChannelName
        std::string inviteMsg = ":" + _users[fd].getPrefix() + " INVITE " + target->getNick() + " :" + channel.getName() + "\r\n";
        return sendToClient(target->getFd(), inviteMsg);
    }
    return sendNumeric(fd, ERR_NEEDMOREPARAMS, "INVITE");
}

void Server::topic(int fd, std::vector<std::string> &params)
{
    if (params.size() < 1)
        throw std::runtime_error("error in Server::topic");
    std::string channel_name = params[0];
    std::map<std::string, Channel>::iterator it = _channels.find(channel_name);
    if (it == _channels.end())
        return sendNumeric(fd, ERR_NOSUCHCHANNEL, channel_name);
    Channel &channel = it->second;
    if (!channel.isMember(fd))
        return sendNumeric(fd, ERR_NOTONCHANNEL, channel_name);

    if (params.size() == 1){
        if (channel.getTopic().content.empty())
            return sendNumeric(fd, RPL_NOTOPIC, channel_name);
        sendNumeric(fd, RPL_TOPIC, channel_name + " " + channel.getTopic().content);
        std::string time_str = timeToStr(channel.getTopic().creationTime);
        return sendNumeric(fd, RPL_TOPICWHOTIME, channel_name + " " + channel.getTopic().setBy + " " + time_str);
    }
    if (!channel.isOperator(fd) && channel.hasMode('t'))
        return sendNumeric(fd, ERR_CHANOPRIVSNEEDED, channel_name);
    channel.setTopic(params[1], _users[fd].getNick());
    // :jade_yuzu!user@host TOPIC #test_room :Coding all night!
    std::string msg = formatNotice(_users[fd], "TOPIC", channel_name + " " + channel.getTopic().content);
    sendToChannel(channel, msg, 0);
}

void Server::msg(int fd, std::vector<std::string> &params)
{
    User &sender = _users[fd];

    if (params.empty() || params[0].empty())
    {
        sendNumeric(fd, ERR_NORECIPIENT, "PRIVMSG");
        return;
    }

    if (params.size() < 2 || params[1].empty())
    {
        sendNumeric(fd, ERR_NOTEXTTOSEND, "PRIVMSG");
        return;
    }

    std::string target = params[0];

    // CHANNEL MESSAGE
    if (target[0] == '#')
    {
        std::map<std::string, Channel>::iterator chanIt = _channels.find(target);

        if (chanIt == _channels.end())
        {
            sendNumeric(fd, ERR_NOSUCHCHANNEL, "PRIVMSG");
            return;
        }

        Channel &channel = chanIt->second;

        if (!channel.isMember(fd) && channel.hasMode('n'))
        {
            sendNumeric(fd, ERR_CANNOTSENDTOCHAN, target);
            return;
        }
        std::string message =
            ":" + sender.getNick() + "!" +
            sender.getUsername() + "@" +
            sender.getHostname() +
            " PRIVMSG " + target + " :" + params[1];

        std::set<int> notified;
        notified.insert(fd);

        sendToChannel(channel, message, notified);
    }
    else
    {
        int targetFd = getFdFromNick(target);

        if (targetFd == -1 || _users.find(targetFd) == _users.end())
        {
            sendNumeric(fd, ERR_NOSUCHNICK, target);
            return;
        }

        std::string user_and_msg = target + " :" + params[1];
        std::string message = formatNotice(sender, "PRIVMSG", user_and_msg);
        sendToClient(targetFd, message);
    }
}

void Server::kick(int fd, std::vector<std::string> &params)
{
    User &user = _users[fd];
    std::string channel_name = params[0];
    std::map<std::string, Channel>::iterator it = _channels.find(channel_name);
    if (it == _channels.end())
        return sendNumeric(fd, ERR_NOSUCHCHANNEL, channel_name);
    User *target = findUserByNick(params[1]);
    if (!target)
        return sendNumeric(fd, ERR_NOSUCHNICK, params[1]);
    Channel &channel = it->second;
    if (!channel.isMember(fd))
        return sendNumeric(fd, ERR_NOTONCHANNEL, channel_name);
    if (!channel.isOperator(fd))
        return sendNumeric(fd, ERR_CHANOPRIVSNEEDED, channel_name);
    if (!channel.isMember(*target))
        return sendNumeric(fd, ERR_USERNOTINCHANNEL, channel_name);
    std::string reason;
    if (params.size() > 2)
        reason = " : " + params[2];
    //:<kicker_prefix> KICK <channel> <target> :<reason>"
    std::string msg = ":" + user.getPrefix() + " KICK " + channel.getName() + " " + target->getNick() + reason;
    sendToChannel(channel, msg, 0);
    channel.removeMember(target->getFd());
    target->removeChannel(channel_name);
}

void Server::notice(int fd, std::vector<std::string> &params)
{
    // 1. Basic validation (Silent return as per RFC 2812)
    std::map<int, User>::iterator itUser = _users.find(fd);
    if (itUser == _users.end() || !itUser->second.isAuthenticated())
        return;
    if (params.size() < 2 || params[0].empty())
        return;

    User &sender = itUser->second;
    std::string targetName = params[0];
    std::string text = params[1];

    // 2. Handle Channel Notice
    if (targetName[0] == '#')
    {
        std::map<std::string, Channel>::iterator itChan = _channels.find(targetName);
        if (itChan == _channels.end())
            return;

        Channel &channel = itChan->second;
        // Optional: Check if sender is in channel, but some IRC setups allow external notice
        if (!channel.isMember(fd))
            return;

        std::string message = ":" + sender.getPrefix() + " NOTICE " + targetName + " :" + text + "\r\n";

        // Broadcast to everyone in channel EXCEPT the sender
        std::set<int> exclude;
        exclude.insert(fd);
        sendToChannel(channel, message, exclude);
    }
    // 3. Handle Private Notice (User to User)
    else
    {
        int targetFd = getFdFromNick(targetName);
        if (targetFd == -1)
            return;

        std::map<int, User>::iterator itTarget = _users.find(targetFd);
        if (itTarget == _users.end())
            return;

        std::string message = ":" + sender.getPrefix() + " NOTICE " + targetName + " :" + text + "\r\n";
        sendToClient(targetFd, message);
    }
}

void Server::list(int fd, std::vector<std::string> &params)
{
    User &user = _users[fd];
    (void)params; // Simplified: list all channels

    // 321 RPL_LISTSTART (Often optional, but good practice)
    sendToClient(fd, formatNumeric(RPL_LISTSTART, user.getNick(), "Channel :Users Name"));

    for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        std::stringstream ss;
        ss << it->first << " " << it->second.getMemberCount();

        // RPL_LIST (322): #channel 5 :[Topic]
        sendToClient(fd, formatNumeric(RPL_LIST, user.getNick(), ss.str() + " :" + it->second.getTopic().content));
    }

    // 323 RPL_LISTEND
    sendToClient(fd, formatNumeric(RPL_LISTEND, user.getNick(), ""));
}

void Server::names(int fd, std::vector<std::string> &params)
{
    User &user = _users[fd];
    if (!user.isAuthenticated())
        return sendToClient(fd, formatNumeric(ERR_NOTREGISTERED, user.getNick(), "NAMES"));

    std::vector<std::string> channelsToShow;
    if (params.empty())
    {
        for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it)
            channelsToShow.push_back(it->first);
    }
    else
    {
        channelsToShow.push_back(params[0]);
    }
    for (size_t i = 0; i < channelsToShow.size(); ++i)
    {
        std::map<std::string, Channel>::iterator it = _channels.find(channelsToShow[i]);
        if (it != _channels.end())
        {
            // RPL_NAMREPLY (353):  = #channel :@op member1 member2
            std::string chanData = "= " + it->first;
            sendToClient(fd, formatNumeric(RPL_NAMREPLY, user.getNick(), chanData + " :" + it->second.getMemberList()));

            // RPL_ENDOFNAMES (366): #channel :End of /NAMES list
            sendToClient(fd, formatNumeric(RPL_ENDOFNAMES, user.getNick(), it->first));
        }
    }
}

void Server::quit(int fd, std::vector<std::string> &params)
{
    std::map<int, User>::iterator itUser = _users.find(fd);
    if (itUser == _users.end())
        return;

    User &user = itUser->second;
    std::string reason = "Client Quit";
    if (!params.empty() && !params[0].empty())
        reason = params[0];

    std::string msg = ":" + user.getPrefix() + " QUIT :" + reason + "\r\n";

    std::vector<std::string> userChannels = user.getChannels();

    std::set<int> notified;
    notified.insert(fd);
    std::vector<std::string> empty_channels;
    for (size_t i = 0; i < userChannels.size(); ++i)
    {
        std::map<std::string, Channel>::iterator it = _channels.find(userChannels[i]);
        if (it == _channels.end())
            continue;

        sendToChannel(it->second, msg, notified);
        it->second.removeMember(fd);

        if (it->second.getMemberCount() == 0)
            empty_channels.push_back(it->second.getName());
    }
    for (size_t i = 0; i < empty_channels.size(); i++)
    {
        _channels.erase(empty_channels[i]);
        std::cout << "Channel " << empty_channels[i] << " deleted (empty)." << std::endl;
    }
    disconnectClient(fd);
}