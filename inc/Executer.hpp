/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 22:44:41 by atambo            #+#    #+#             */
/*   Updated: 2026/02/26 18:49:34 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTER_HPP
#define EXECUTER_HPP

#include <map>
#include <sstream>
// ---------------------------------
class User;
class Channel;
class Server;
class Executer;
struct Command
{
    typedef void (Executer::*Handler)(void);
    Handler handler;
    short param_num;
    bool need_trail;
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
    void execute(User *sender, std::string rawCommand);
};

#endif
