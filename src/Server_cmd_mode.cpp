/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_cmd_mode.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 12:29:57 by atambo            #+#    #+#             */
/*   Updated: 2026/03/14 11:17:50 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

void Server::mode(int fd, std::vector<std::string> &params)
{
    std::string target = params[0];
    if (target[0] != '&' && target[0] != '#')
    {
        std::map<int, User>::iterator it = _users.find(fd);
        if (it == _users.end())
            return sendNumeric(fd, ERR_NOSUCHNICK, target);
        return sendNumeric(fd, RPL_UMODEIS, " +");
    }

    std::map<std::string, Channel>::iterator it = _channels.find(target);
    if (it == _channels.end())
        return sendNumeric(fd, ERR_NOSUCHCHANNEL, target);

    Channel &channel = (it->second);
    if (!channel.isMember(fd))
        return sendNumeric(fd, ERR_NOTONCHANNEL, target);
    if (params.size() == 1)
    {
        sendNumeric(fd, RPL_CHANNELMODEIS, target + " " + channel.getModeStr());
        return sendNumeric(fd, RPL_CREATIONTIME, target + " " + timeToStr(channel.getCreationTime()));
    }
    if (params.size() > 1 && !params[1].empty())
    {
        if (!channel.isOperator(fd))
            return sendNumeric(fd, ERR_CHANOPRIVSNEEDED, target);

        applyModeString(fd, params, channel);  
    }
}

void Server::applyModeString(int fd, std::vector<std::string> &params, Channel &channel)
{
    if (params.size() < 2) return; // Safety check
    
    std::string modes = params[1];
    bool adding = true;
    size_t j = 2; // Parameter index

    std::string old_modes = channel.getModeStr();
    for (size_t i = 0; i < modes.length(); ++i)
    {
        char c = modes[i];
        if (c == '+') { adding = true; continue; }
        if (c == '-') { adding = false; continue; }

        switch (c)
        {
            case 'k':
                if (adding){
                    if (j < params.size()) {
                        mode_k(fd, params, channel, j++, adding);
                    }
                }
                else{
                    channel.unsetMode('k');
                    channel.unsetKey();
                }
                break;

            case 'o':
                if (j < params.size()) {
                    mode_o(fd, params, channel, j++, adding);
                } else {
                    sendNumeric(fd, ERR_NEEDMOREPARAMS, "MODE +o");
                }
                break;

            case 'l':
                if (adding) {
                    if (j < params.size()) {
                        mode_l(fd, params, channel, j++, adding);
                    }
                } else {
                    channel.unsetMode('l');
                    channel.unsetLimit();
                }
                break;

            case 'i':
            case 't':
            case 'n':
                adding ? channel.setMode(c) : channel.unsetMode(c);
                break;

            default:
                std::string modeChar(1, c);
                sendNumeric(fd, ERR_UNKNOWNMODE, modeChar);
                break;
        }
    }
    if (old_modes != channel.getModeStr()){
        std::string msg = formatNotice(_users[fd], "MODE", channel.getModeStr());
        sendToChannel(channel, msg, 0);    
    }
}

bool Server::mode_k(int fd, std::vector<std::string> &params, Channel &channel, size_t j, bool adding)
{

    if (params.size() <= j)
    {
        sendNumeric(fd, ERR_INVALIDMODEPARAM, channel.getName() + " +k missing key parameter");
        return 1;
    }
    if (params[j].find_first_of(" ") != params[j].npos)
    {

        sendNumeric(fd, ERR_INVALIDMODEPARAM, channel.getName() + " +k '" + params[j]);
        return 1;
    }
    if (params[j].size() > MAX_CHAN_KEY_LEN)
    {
        std::stringstream ss;
        ss << " +k '" << params[j] << "' key must not exceed "
           << MAX_CHAN_KEY_LEN << " characters";

        sendNumeric(fd, ERR_INVALIDMODEPARAM, channel.getName() + ss.str());
    }
    if (adding)
    {
        channel.setKey(params[j]);
        channel.setMode('k');
    }
    else
    {
        channel.unsetKey();
        channel.unsetMode('k');
    }
    return 0;
}

bool Server::mode_o(int fd, std::vector<std::string> &params, Channel &channel, size_t j, bool make_operator)
{
    if (j >= params.size())
    {
        sendNumeric(fd, ERR_NEEDMOREPARAMS, "MODE +o is missing a parameter");
        return true;
    }

    User *target = findUserByNick(params[j]);
    if (!target)
    {
        sendNumeric(fd, ERR_NOSUCHNICK, params[j]);
        return true;
    }

    if (!channel.isMember(target->getFd()))
    {
        sendNumeric(fd, ERR_USERNOTINCHANNEL, params[j]);
        return true;
    }

    channel.setOperator(target->getFd(), make_operator);
    std::string msg = ":" + _users[fd].getNick() + " MODE " + channel.getName() + (make_operator ? " +o " : " -o ") + target->getNick();
    sendToChannel(channel, msg, fd);
    return false;
}

bool Server::mode_l(int fd, std::vector<std::string> &params, Channel &channel, size_t j, bool adding)
{
    if (adding)
    {
        if (j >= params.size())
        {
            sendNumeric(fd, ERR_NEEDMOREPARAMS, "MODE +l missing limit parameter");
            return false;
        }
        int new_limit = std::atoi(params[j].c_str());

        if (new_limit <= 0)
            return true;

        channel.setLimit(new_limit);
        channel.setMode('l');
    }
    else
    {
        channel.unsetLimit();
        channel.unsetMode('l');
    }

    // 4. Broadcast the change to the channel
    std::string modeChar = adding ? "+l" : "-l";
    std::string limitStr = adding ? params[j] : "";
    std::string msg = ":" + _users[fd].getNick() + " MODE " + channel.getName() + " " + modeChar + " " + limitStr;
    sendToChannel(channel, msg, fd);

    return true; // Successfully processed
}