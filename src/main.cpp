/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 09:19:53 by mvidal            #+#    #+#             */
/*   Updated: 2026/03/05 15:16:00 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>
#include <sys/types.h>
# include <signal.h>
#include <iostream>
#include <cstdlib>

#include "../inc/Server.hpp"

Server*	gserver = NULL;

void	sighandler(int signum)
{
	(void)signum;
	if (gserver)
		gserver->turnOff();
}

int main(int ac, char** av) {
    std::string		password;
    unsigned short	port;
    char			c;

	if (ac != 3)
        return (std::cerr << "Error: bad arguments." << std::endl, 1);

	std::stringstream ss(av[1]);
	if (!(ss >> port) || ss >> c)
		return (std::cerr << "Error: invalid port." << std::endl, 1);

	password = av[2];
	try {
		signal(SIGINT, sighandler);
		Server server(port, password);
		gserver = &server;
		server.listenMode();
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}

	return (0);
}
