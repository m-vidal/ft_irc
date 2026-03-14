/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 12:37:24 by atambo            #+#    #+#             */
/*   Updated: 2026/03/14 10:38:24 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

User *Server::findUserByNick(const std::string &nick)
{
    for (size_t i = 1; i < _polls.size(); ++i)
    {
        int fd = _polls[i].fd;

        // opcional: ignorar o fd do próprio servidor se estiver no índice 0
        if (_users.find(fd) == _users.end())
            continue;

        if (_users[fd].getNick() == nick)
            return (&_users[fd]);
    }
    return (NULL);
}

std::string Server::getUserNick(int fd) const
{
    std::map<int, User>::const_iterator it = _users.find(fd);
    if (it == _users.end() || it->second.isAuthenticated() == false || it->second.getNick() == "*")
    {
        return "*";
    }
    return (it->second.getNick());
}

int Server::getFdFromNick(std::string nick)
{
    for (size_t i = 1; i < _polls.size(); i++)
    {
        if (_users[_polls[i].fd].getNick() != nick)
            continue;
        return (_users[_polls[i].fd].getFd());
    }
    return (-1);
}

std::vector<std::string> split(const std::string &input)
{
    std::vector<std::string> result;
    std::string token;
    std::string::size_type i = 0;
    const std::string::size_type n = input.size();

    while (i < n && (input[i] == ' ' || input[i] == '\t'))
        ++i;

    while (i < n)
    {
        token.clear();
        while (i < n && input[i] != ' ' && input[i] != '\t')
        {
            token += input[i];
            ++i;
        }

        if (!token.empty())
            result.push_back(token);
        while (i < n && (input[i] == ' ' || input[i] == '\t'))
            ++i;
    }
    return (result);
}

bool Server::checkPassword(std::string password)
{
    // bool hasUpper = false;
    // bool hasLower = false;
    // bool hasNumbr = false;
    // bool hasSmbl = false;

    // if (password.size() < 8)
    // 	return (false);
    // for (size_t i = 0; i < password.size(); i++)
    // {
    // 	if (std::isupper(password[i]))
    // 		hasUpper = true;

    // 	else if (std::islower(password[i]))
    // 		hasLower = true;
    // 	else if (std::isdigit(password[i]))
    // 		hasNumbr = true;
    // 	else
    // 		hasSmbl = true;
    // }
    // if (hasUpper && hasLower && hasNumbr && hasSmbl)
    // 	return (true);
    // return (false);
    (void)password;
    return true;
}

void Server::printBanner()
{
    std::string colors[] = {
        "\033[31m",
        "\033[32m",
        "\033[33m",
        "\033[34m",
        "\033[35m",
        "\033[36m",
        "\033[91m",
        "\033[92m",
        "\033[93m",
        "\033[94m",
        "\033[95m",
        "\033[96m"};
    int num_colors = 12;

    std::srand(std::time(0));
    std::string banner_color = colors[std::rand() % num_colors];
    std::string accent_color = colors[std::rand() % num_colors];
    std::string reset = "\033[0m";

    std::cout << banner_color;
    std::cout << "  _____ _____   _____  _____ ______ _______      __" << std::endl;
    std::cout << " |_   _|  __ \\ / ____|/ ____|  ____|  __ \\ \\    / /" << std::endl;
    std::cout << "   | | | |__) | |    | (___ | |__  | |__) \\ \\  / / " << std::endl;
    std::cout << "   | | |  _  /| |     \\___ \\|  __| |  _  / \\ \\/ /  " << std::endl;
    std::cout << "  _| |_| | \\ \\| |____ ____) | |____| | \\ \\  \\  /   " << std::endl;
    std::cout << " |_____|_|  \\_\\\\_____|_____/|______|_| _\\_\\  \\/    " << std::endl;
    std::cout << reset << std::endl;
    std::cout << accent_color << " [SYSTEM] " << reset << "ft_irc server started" << std::endl;
    std::cout << accent_color << " [INFO]   " << reset << "Listening on port: " << _port << std::endl;
    std::cout << " --------------------------------------------------" << std::endl;
}

bool valid_channel_name(const std::string &name)
{
    if (name.empty() || name.size() > 200)
        return false;

    if (name[0] != '#' && name[0] != '&')
        return false;

    // " " = Space
    // "," = Comma
    // "\x07" = Control G (Hex for ASCII 7)
    if (name.find_first_of(" ,\x07") != std::string::npos)
        return false;

    return true;
}

std::string timeToStr(time_t time)
{
    std::stringstream ss;
    ss << time;
    return ss.str();
}

std::string ircToLower(std::string str)
{
    for (size_t i = 0; i < str.length(); ++i)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] += 32;
        else if (str[i] == '[')
            str[i] = '{';
        else if (str[i] == ']')
            str[i] = '}';
        else if (str[i] == '\\')
            str[i] = '|';
        else if (str[i] == '~')
            str[i] = '^';
    }
    return str;
}

void Server::initReplies()
{
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

    // Nickname & Registration Errors
    _replyMessages[ERR_NONICKNAMEGIVEN] = "No nickname given";
    _replyMessages[ERR_ERRONEUSNICKNAME] = "Erroneous nickname";
    _replyMessages[ERR_NICKNAMEINUSE] = "Nickname is already in use";
    _replyMessages[ERR_NOTREGISTERED] = "You have not registered";
    _replyMessages[ERR_ALREADYREGISTERED] = "Unauthorized command (already registered)";
    _replyMessages[ERR_PASSWDMISMATCH] = "Password incorrect";

    // Misc Errors
    _replyMessages[ERR_UNKNOWNCOMMAND] = "Unknown command";
    _replyMessages[ERR_NEEDMOREPARAMS] = "Not enough parameters";
    _replyMessages[ERR_INVALIDMODEPARAM] = "Invalid mode parameter";
}

const std::string &Server::getNumericMsg(int code)
{
    std::map<int, std::string>::const_iterator it = _replyMessages.find(code);
    if (it != _replyMessages.end())
        return it->second;

    // Fallback if code isn't defined to prevent crashes
    static std::string fallback = "";
    return fallback;
}

time_t Channel::getCreationTime() const
{
    return _creationTime;
}

std::string Server::getCreationTimeStr(time_t time) const
{

    return timeToStr(time);
}
