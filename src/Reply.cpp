/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 14:24:18 by atambo            #+#    #+#             */
/*   Updated: 2026/03/06 14:24:33 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

void Server::ircReply(int fd, int code, const std::string &command, const std::string &trailing) const
{
    std::stringstream ss;

    ss << std::setw(3) << std::setfill('0') << code;
    std::string reply = ":ircserv " + ss.str() + " " + getUserNick(fd) + " " + command + " :" + trailing + "\r\n";
    send(fd, reply.c_str(), reply.size(), 0);
}
void Server::ircReply(int fd, const std::string &msg) const
{
    std::string reply = msg + "\r\n";
    send(fd, reply.c_str(), reply.size(), 0);
}
void Server::ircReply(int fd, const std::string &command, const std::string &trailing) const
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