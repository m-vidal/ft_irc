/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reply_codes.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 07:38:07 by atambo            #+#    #+#             */
/*   Updated: 2026/04/26 11:52:06 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

void Server::initReplies()
{
    _replyMessages[ERR_NOTEXTTOSEND] = "No text to send";
    // replies
    _replyMessages[RPL_ENDOFINVITELIST] = "End of /INVITE list";
    _replyMessages[RPL_ENDOFNAMES] = "End of /NAMES list";
    _replyMessages[RPL_LISTEND] = "End of /LIST";
    // Channel & Mode Errors
    _replyMessages[ERR_NOSUCHNICK] = "No such nick/channel";
    _replyMessages[ERR_NOSUCHCHANNEL] = "No such channel";
    _replyMessages[ERR_CANNOTSENDTOCHAN] = "Cannot send to channel";
    _replyMessages[ERR_TOOMANYCHANNELS] = "You have joined too many channels";
    _replyMessages[ERR_CHANNELISFULL] = "Cannot join channel (+l)";
    _replyMessages[ERR_INVITEONLYCHAN] = "Cannot join channel (+i)";
    _replyMessages[ERR_BADCHANNELKEY] = "Cannot join channel (+k)";
    _replyMessages[ERR_CHANOPRIVSNEEDED] = "You're not channel operator";
    _replyMessages[ERR_USERNOTINCHANNEL] = "They aren't on that channel";
    _replyMessages[ERR_NOTONCHANNEL] = "You're not on that channel";
    _replyMessages[ERR_USERONCHANNEL] = "is already on channel";
    _replyMessages[ERR_UNKNOWNMODE] = "is unknown mode char to me";
    _replyMessages[ERR_BADCHANMASK] = "Bad Channel Mask";
    _replyMessages[ERR_CANNOTSENDTOCHAN] = "Cannot send to channel";
    _replyMessages[RPL_NOTOPIC] = "No topic is set";
    // Nickname & Registration Errors
    _replyMessages[ERR_NONICKNAMEGIVEN] = "No nickname given";
    _replyMessages[ERR_ERRONEUSNICKNAME] = "Erroneous nickname";
    _replyMessages[ERR_NICKNAMEINUSE] = "Nickname is already in use";
    _replyMessages[ERR_NOTREGISTERED] = "You have not registered";
    _replyMessages[ERR_ALREADYREGISTERED] = "You are already connected and cannot handshake again";
    _replyMessages[ERR_PASSWDMISMATCH] = "Password incorrect";

    // Misc Errors
    _replyMessages[ERR_UNKNOWNCOMMAND] = "Unknown command";
    _replyMessages[ERR_NEEDMOREPARAMS] = "Not enough parameters";
    _replyMessages[ERR_INVALIDMODEPARAM] = "Invalid mode parameter";
    _replyMessages[ERR_NORECIPIENT] = "No recipient given PRIVMSG";
}