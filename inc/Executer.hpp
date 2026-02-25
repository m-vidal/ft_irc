/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 22:44:41 by atambo            #+#    #+#             */
/*   Updated: 2026/02/24 17:09:33 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef EXECUTER_HPP
# define EXECUTER_HPP

#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"

class Executor {
    private:
    
        Server &_server;
        typedef void (Executor::*CommandFunc)(User&, const std::vector<std::string>&);
        std::unordered_map<std::string, CommandFunc> _cmd;
        std::vector<std::string>	parser(std::string &rawCommand);
        
        //std::map<std::string, CommandFunc> _commands;

        void	Join(User& sender, const std::vector<std::string>& params);
        void	Privmsg(User& sender, const std::vector<std::string>& params);
        void	Topic(User& sender, const std::vector<std::string>& params);

    public:
        Executor(Server& server);
        void execute(User& sender, std::string rawCommand);
};

#endif
