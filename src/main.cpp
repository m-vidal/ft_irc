/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 09:19:53 by mvidal            #+#    #+#             */
/*   Updated: 2026/04/26 09:11:06 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

volatile sig_atomic_t kill_server = 0;

void signalHandler(int signum) {
    (void)signum;
    kill_server = 1; // Set the flag
}

int main(int ac, char **av)
{
	// Ignore SIGPIPE to stop the crashes you saw earlier
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, signalHandler);
    signal(SIGQUIT, signalHandler);
	
	std::string password;
	long long port_check = 0;
	unsigned short port;
	char c;

	if (ac != 3)
		return (std::cerr << "Error: bad arguments." << std::endl, 1);

	std::stringstream ss(av[1]);
	if (!(ss >> port_check) || ss.fail() || ss >> c  || port_check < PORT_MIN || port_check > PORT_MAX)
		return (std::cerr << "Error: invalid port '" << av[1] << "'.\nPort number must be between " << PORT_MIN << "-" << PORT_MAX << std::endl, 1);
	port = static_cast<unsigned short>(port_check);
	std::cout << "IRC Server starting..." << std::endl;

	password = av[2];
	try
	{
		Server server(port, password, "ircserv");
		server.listenMode();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	return (0);
}
