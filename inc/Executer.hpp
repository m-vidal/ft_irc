/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 22:44:41 by atambo            #+#    #+#             */
/*   Updated: 2026/02/27 15:56:54 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTER_HPP
#define EXECUTER_HPP

#include <map>
#include <vector>
#include <sstream>
#include <iostream>
// ---------------------------------

class User;
class Channel;
class Server;
class Executer;

typedef void (Executer::*Handler)(void);
struct Command
{
    Handler handler;
    int minArgs;
    bool requiresAuth;

    Command();
    Command(Handler h, int args, bool auth);
};

class Executer
{
private:
    Server &_server;
    User *_user;
    std::map<std::string, Command> _cmdHandlers;
    void parser(std::string &rawCommand);
    // cmd data ------------------------------------------
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
    void processMessage(User *sender, std::string rawCommand);
};

#endif
