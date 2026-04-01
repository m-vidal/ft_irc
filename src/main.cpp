/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 09:19:53 by mvidal            #+#    #+#             */
/*   Updated: 2026/04/01 13:09:59 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(int ac, char **av)
{
	std::string password;
	unsigned short port;
	char c;

	if (ac != 3)
		return (std::cerr << "Error: bad arguments." << std::endl, 1);

	std::stringstream ss(av[1]);
	if (!(ss >> port) || ss >> c || ss.fail() || port < 1024 || port > 65535)
		return (std::cerr << "Error: invalid port." << std::endl, 1);

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
