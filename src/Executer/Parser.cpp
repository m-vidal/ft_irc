/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 18:17:59 by atambo            #+#    #+#             */
/*   Updated: 2026/02/25 19:30:12 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Executer.hpp"

void Executer::parser(std::string &rawCommand)
{
    /* split rawCommand into command and params
     * return vector of command and params
     */
    // = /CMD <arg1> <arg2> : <trailing 1> <trailing 2>
    std::string trimmed;
    if (rawCommand.size() >= 2)
        trimmed = rawCommand.substr(0, rawCommand.find("\r\n")); // trimming the \r\n

    std::size_t commaPos = trimmed.find(":");
    std::string trailing;

    trailing = "\0";
    if (commaPos != std::string::npos)
    {
        trailing = trimmed.substr(commaPos + 1); // trailing
    }

    std::string strBeforeTrailing = trimmed.substr(0, commaPos); // COMMAND param1 param2 ....

    std::string command;
    std::vector<std::string> params;

    std::size_t spacePos = strBeforeTrailing.find(' ');

    command = strBeforeTrailing.substr(0, spacePos);
    std::size_t commandLength = command.length();
    for (std::size_t i = 0; i < commandLength; i++)
    {
        command[i] = std::toupper(command[i]);
    }
    std::string tempArgs = strBeforeTrailing.substr(spacePos + 1);
    std::vector<std::string> args = split(tempArgs);
    // -------------------------------------------------
    _cmd = command;
    _params = args;
    param_count = args.size();
    _trailing = trailing;
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
    return result;
}