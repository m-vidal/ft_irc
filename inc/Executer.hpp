/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 22:44:41 by atambo            #+#    #+#             */
/*   Updated: 2026/02/25 19:35:01 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTER_HPP
#define EXECUTER_HPP

#include <map>
#include <sstream>
// ---------------------------------
#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"

class Executer
{
private:
    Server &_server;
    User *_user;
    typedef void (Executer::*CommandFunc)(void);
    std::map<std::string, CommandFunc> _cmdHandlers;
    void parser(std::string &rawCommand);
    // cmd data ------------------------------------------
    unsigned int param_count;
    std::string _cmd;
    std::vector<std::string> _params;
    std::string _trailing;
    // cmd handlers --------------------------------------
    void unknowCmd();
    void join();
    void privmsg();
    void topic();

public:
    Executer(Server &server);
    void execute(User *sender, std::string rawCommand);
};

#endif
