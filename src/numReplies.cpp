/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numReplies.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 14:56:16 by atambo            #+#    #+#             */
/*   Updated: 2026/03/02 18:40:16 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "numReply.hpp"

namespace Reply
{
    /* --- Core Utilities --- */

    std::string numeric(const std::string &srv, const char *code, const std::string &target, const std::string &msg)
    {
        return ":" + srv + " " + code + " " + target + " :" + msg + "\r\n";
    }

    std::string prefix(const User &u)
    {
        return u.getNick() + "!" + u.getName() + "@" + u.getHost();
    }

    std::string cmd(const std::string &sender, const std::string &command, const std::string &params, const std::string &trailing)
    {
        std::string res = ":" + sender + " " + command;
        if (!params.empty())
            res += " " + params;
        if (!trailing.empty())
            res += " :" + trailing;
        res += "\r\n";
        return res;
    }

    // A private helper to keep the trailing messages in one place
    // This makes it easy to change "No such channel" to "Channel not found" everywhere at once
    static std::string get_err_msg(const std::string &code, const std::string &arg)
    {
        if (code == ERR_NOSUCHCHANNEL)
            return arg + " :No such channel";
        if (code == ERR_NICKNAMEINUSE)
            return arg + " :Nickname is already in use";
        if (code == ERR_UNKNOWNCOMMAND)
            return arg + " :Unknown command";
        if (code == ERR_NEEDMOREPARAMS)
            return arg + " :Not enough parameters";
        if (code == ERR_CHANOPRIVSNEEDED)
            return arg + " :You're not channel operator";
        return arg + " :Unknown error";
    }

    static std::string createError(const std::string &srv, const char *code, const std::string &nick, const std::string &arg)
    {
        return numeric(srv, code, nick, get_err_msg(code, arg));
    }
}

namespace Reply
{
    /* --- Event Replies --- */

    std::string join(const User &u, const std::string &channel)
    {
        return cmd(prefix(u), "JOIN", "", channel);
    }

    std::string privmsg(const User &u, const std::string &target, const std::string &msg)
    {
        return cmd(prefix(u), "PRIVMSG", target, msg);
    }

    std::string nick(const std::string &oldPrefix, const std::string &newNick)
    {
        return ":" + oldPrefix + " NICK :" + newNick + "\r\n";
    }

    std::string part(const User &u, const std::string &channel, const std::string &reason)
    {
        return cmd(prefix(u), "PART", channel, reason);
    }
}

namespace Reply
{
    /* --- Handshake & Info --- */

    std::string welcome(const std::string &srv, const std::string &nick)
    {
        return numeric(srv, RPL_WELCOME, nick, "Welcome to the Internet Relay Network " + nick);
    }

    std::string yourhost(const std::string &srv, const std::string &nick)
    {
        return numeric(srv, RPL_YOURHOST, nick, "Your host is " + srv + ", running version 1.0");
    }

    std::string topic(const std::string &srv, const std::string &nick, const std::string &chan, const std::string &topicText)
    {
        return ":" + srv + " " + RPL_TOPIC + " " + nick + " " + chan + " :" + topicText + "\r\n";
    }

    std::string namreply(const std::string &srv, const std::string &nick, const std::string &chan, const std::string &users)
    {
        return ":" + srv + " " + RPL_NAMREPLY + " " + nick + " = " + chan + " :" + users + "\r\n";
    }

    std::string endofnames(const std::string &srv, const std::string &nick, const std::string &chan)
    {
        return numeric(srv, RPL_ENDOFNAMES, nick, chan + " :End of /NAMES list");
    }
}

namespace Reply
{
    /* --- Public Error Wrappers --- */

    std::string err_nicknameinuse(const std::string &srv, const std::string &nick)
    {
        return createError(srv, ERR_NICKNAMEINUSE, nick, nick);
    }

    std::string err_needmoreparams(const std::string &srv, const std::string &nick, const std::string &cmd)
    {
        return createError(srv, ERR_NEEDMOREPARAMS, nick, cmd);
    }

    std::string err_nosuchchannel(const std::string &srv, const std::string &nick, const std::string &chan)
    {
        return createError(srv, ERR_NOSUCHCHANNEL, nick, chan);
    }

    std::string err_chanoprivsneeded(const std::string &srv, const std::string &nick, const std::string &chan)
    {
        return createError(srv, ERR_CHANOPRIVSNEEDED, nick, chan);
    }

    std::string err_unknowncommand(const std::string &srv, const std::string &nick, const std::string &cmd)
    {
        return createError(srv, ERR_UNKNOWNCOMMAND, nick, cmd);
    }

}