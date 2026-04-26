/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replyNotice.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 14:24:18 by atambo            #+#    #+#             */
/*   Updated: 2026/04/26 07:37:53 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

// For Commands (e.g., :nick!user@host JOIN #channel)
std::string Server::formatNotice(const User &source, const std::string &command, const std::string &params)
{
    std::string msg = ":" + source.getPrefix() + " " + command;
    if (!params.empty())
        msg += " " + params;
    return msg;
}

void Server::sendNotice(int fd, const User &source, const std::string &command, const std::string &params)
{
    std::string msg = formatNotice(source, command, params);
    sendToClient(fd, msg);
}



