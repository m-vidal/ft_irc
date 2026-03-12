/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_cmd_mode.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 12:29:57 by atambo            #+#    #+#             */
/*   Updated: 2026/03/12 15:17:10 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

void Server::mode(int fd, std::vector<std::string> &params, std::string trailing)
{
    (void)fd;
    (void)params;
    (void)trailing;
    std::string channel_name = params[0];
    std::map<std::string, Channel>::iterator it = _channels.find(channel_name);
    if (it == _channels.end())
        return sendNumeric(fd, ERR_NOSUCHCHANNEL, channel_name, "No such channel!");

    Channel &channel = (it->second);
    if (!channel.isMember(fd))
        return sendNumeric(fd, ERR_NOTONCHANNEL, channel_name, "You're not on that channel.");
    if (params.size() == 1)
    {
        sendNumeric(fd, RPL_CHANNELMODEIS, channel_name, channel.getModeStr());
        return sendNumeric(fd, RPL_CREATIONTIME, channel_name, channel.getCreationTimeStr());
    }
    if (params.size() > 1)
    {
        if (!channel.isOperator(fd))
            return sendNumeric(fd, ERR_CHANOPRIVSNEEDED, channel_name, "You're not channel operator");

        applyModeString(fd, params, trailing, channel);
    }
}

bool Server::mode_k(int fd, std::vector<std::string> &params, Channel &channel, size_t j)
{

    if (params.size() <= j)
    {
        sendNumeric(fd, ERR_INVALIDMODEPARAM, channel.getName(), " +k missing key parameter");
        return 1;
    }
    if (params[j].find_first_of(" ") != params[j].npos)
    {

        sendNumeric(fd, ERR_INVALIDMODEPARAM, channel.getName(), " +k '" + params[j] + "' key must not have spaces");
        return 1;
    }
    if (params[j].size() > MAX_CHAN_KEY_LEN)
    {
        std::stringstream ss;
        ss << " +k '" << params[j] << "' key must not exceed "
           << MAX_CHAN_KEY_LEN << " characters";

        sendNumeric(fd, ERR_INVALIDMODEPARAM, channel.getName(), ss.str());
    }
    channel.setKey(params[j]);
    return 0;
}

void Server::applyModeString(int fd, std::vector<std::string> &params, std::string trailing, Channel &channel)
{
    (void)trailing;
    bool adding = true;
    std::string modes = params[1];
    for (size_t i = 0; i < modes.length(); ++i)
    {
        size_t j = 2;
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
                if (mode_k(fd, params, channel, j))
                    return;
                j++;
            }
            if (!channel.hasMode(c))
            {
                channel.setMode(c);
                sendNumeric(channel, fd, RPL_CHANNELMODEIS, channel.getName(), channel.getModeStr());
            }
        }
        else
        {
            if (channel.hasMode(c))
            {
                channel.unsetMode(c);
                sendNumeric(channel, fd, RPL_CHANNELMODEIS, channel.getName(), channel.getModeStr());
            }
        }
    }
}