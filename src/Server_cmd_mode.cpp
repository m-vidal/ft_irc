/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_cmd_mode.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 12:29:57 by atambo            #+#    #+#             */
/*   Updated: 2026/03/13 17:22:57 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

void Server::mode(int fd, std::vector<std::string> &params)
{
    std::string channel_name = params[0];
    std::map<std::string, Channel>::iterator it = _channels.find(channel_name);
    if (it == _channels.end())
        return sendNumeric(fd, ERR_NOSUCHCHANNEL, channel_name);

    Channel &channel = (it->second);
    if (!channel.isMember(fd))
        return sendNumeric(fd, ERR_NOTONCHANNEL, channel_name);
    if (params.size() == 1)
    {
        sendNumeric(fd, RPL_CHANNELMODEIS, channel_name + " " + channel.getModeStr());
        return sendNumeric(fd, RPL_CREATIONTIME, channel_name + " " + channel.getCreationTimeStr());
    }
    if (params.size() > 1)
    {
        if (!channel.isOperator(fd))
            return sendNumeric(fd, ERR_CHANOPRIVSNEEDED, channel_name);

        applyModeString(fd, params, channel);
    }
}

void Server::applyModeString(int fd, std::vector<std::string> &params, Channel &channel)
{
    std::string modes = params[1];
    bool adding = true;
    size_t j = 2; // Parameter index starts here

    for (size_t i = 0; i < modes.length(); ++i)
    {
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

        switch (c)
        {
        // Modes that require a parameter
        case 'k':
            mode_k(fd, params, channel, j, adding);
            if (adding)
                j++;
            break;

        case 'o':
            mode_o(fd, params, channel, j, adding);
            j++; // o always consumes a parameter
            break;

        case 'l':
            mode_l(fd, params, channel, j, adding);
            if (adding)
                j++;
            break;

        // Simple toggle modes (no parameters)
        case 'i':
        case 't':
            adding ? channel.setMode(c) : channel.unsetMode(c);
            break;

        default:
            sendNumeric(fd, ERR_UNKNOWNMODE, std::string(1, c));
            break;
        }
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