/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel_mode.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 08:26:11 by atambo            #+#    #+#             */
/*   Updated: 2026/03/10 08:32:52 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"

bool Channel::isModeChar(const char c)
{
    std::string mode_chars = "itkol";
    return mode_chars.find(c) != mode_chars.npos;
}

void Channel::setMode(const char c)
{
    if (isModeChar(c))
        _mode[c] = true;
}

void Channel::unsetMode(const char c)
{
    if (isModeChar(c))
        _mode[c] = false;
}

// Since you moved to a map, this likely returns the internal map now
std::map<char, bool> Channel::getMode() const
{
    return (_mode);
}

bool Channel::hasMode(const char c) const
{
    // Use find to avoid creating a new entry in the map if the key doesn't exist
    std::map<char, bool>::const_iterator it = _mode.find(c);
    if (it != _mode.end())
        return it->second;
    return false;
}

std::string Channel::getModeStr() const
{
    std::string modeStr = "+";
    std::map<char, bool>::const_iterator it;

    // Iterate through the map and add only the keys that are true
    for (it = _mode.begin(); it != _mode.end(); ++it)
    {
        if (it->second)
            modeStr += it->first;
    }

    return (modeStr == "+" ? "" : modeStr);
}

void Channel::applyModeString(const std::string &modes)
{
    bool adding = true;

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

        if (adding)
            this->setMode(c);
        else
            this->unsetMode(c);
    }
}