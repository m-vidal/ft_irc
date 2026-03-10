/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 12:37:24 by atambo            #+#    #+#             */
/*   Updated: 2026/03/10 12:39:46 by atambo           ###   ########.fr       */
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
    std::string cyan = "\033[36m";
    std::string green = "\033[32m";
    std::string reset = "\033[0m";

    std::cout << cyan;
    std::cout << "  _____ _____   _____  _____ ______ _______      __" << std::endl;
    std::cout << " |_   _|  __ \\ / ____|/ ____|  ____|  __ \\ \\    / /" << std::endl;
    std::cout << "   | | | |__) | |    | (___ | |__  | |__) \\ \\  / / " << std::endl;
    std::cout << "   | | |  _  /| |     \\___ \\|  __| |  _  / \\ \\/ /  " << std::endl;
    std::cout << "  _| |_| | \\ \\| |____ ____) | |____| | \\ \\  \\  /   " << std::endl;
    std::cout << " |_____|_|  \\_\\\\_____|_____/|______|_| _\\_\\  \\/    " << std::endl;
    std::cout << reset << std::endl;

    std::cout << green << " [SYSTEM] " << reset << "ft_irc server started" << std::endl;
    std::cout << green << " [INFO]   " << reset << "Listening on port: " << _port << std::endl;
    std::cout << " --------------------------------------------------" << std::endl;
}