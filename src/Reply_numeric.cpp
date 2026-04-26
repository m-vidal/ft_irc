/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply_numeric.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 07:36:40 by atambo            #+#    #+#             */
/*   Updated: 2026/04/26 08:25:43 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

// For Numerics (e.g., :ircserv 001 target :Welcome)
std::string Server::formatNumeric(int code, const std::string &nick, std::vector<std::string> params)
{
    std::stringstream ss;

    ss << ":" << _serverName << " " << std::setw(3) << std::setfill('0') << code << " " << nick;

    // Add the mapped message from your map
    std::string msg = getNumericMsg(code);
    if (!msg.empty())
        params.push_back(msg);

    for (size_t i = 0; i < params.size(); ++i)
    {
        ss << " ";
        if (i == params.size() - 1)
        {
            if (params[i].find(' ') != std::string::npos || (!params[i].empty() && params[i][0] == ':'))
                ss << ":";
        }
        ss << params[i];
    }

    ss << "\r\n";
    return ss.str();
}

void Server::sendNumeric(int fd, int code,  const std::string& p1, 
                                            const std::string& p2,
                                            const std::string& p3) 
{
    std::vector<std::string> params;
    
    if (!p1.empty()) params.push_back(p1);
    if (!p2.empty()) params.push_back(p2);
    if (!p3.empty()) params.push_back(p3);

    // Uses your existing lookup logic for the nick
    std::string nick = _users[fd].getNick();
    if (nick.empty()) nick = "*";

    std::string msg = formatNumeric(code, nick, params);
    send(fd, msg.c_str(), msg.length(), 0);
}

void Server::sendNumeric(Channel &channel, int fd, int code, const std::string &params)
{
    // 1. Get the nick of the user who triggered this (or "*" if not set)
    std::string nick = _users[fd].getNick();
    if (nick.empty())
        nick = "*";

    // 2. Split the params string into tokens to avoid the "accidental colon" bug
    std::vector<std::string> pList;
    std::stringstream ss(params);
    std::string token;
    while (ss >> token)
        pList.push_back(token);

    // 3. Generate the formatted numeric string
    // formatNumeric will append the map message and handle the trailing colon correctly
    std::string msg = formatNumeric(code, nick, pList);

    // 4. Broadcast to the channel, skipping the person who triggered it
    this->sendToChannel(channel, msg, fd);
}