/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply_notice.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 14:24:18 by atambo            #+#    #+#             */
/*   Updated: 2026/04/28 15:57:23 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

// For Commands (e.g., :nick!user@host JOIN #channel)
std::string Server::formatNotice(const User &source, const std::string &command,
    const std::string &param1, const std::string &param2, const std::string &param3)
{
    std::string msg = ":" + source.getPrefix() + " " + command;
    if (!param1.empty())
        msg += " " + param1;
    if (!param2.empty())
        msg += " " + param2;
    if (!param3.empty())
        msg += " " + param3;
    return msg;
}

void Server::sendNotice(int fd, const User &source, const std::string &command, const std::string &params)
{
    std::string msg = formatNotice(source, command, params);
    sendToClient(fd, msg);
}



