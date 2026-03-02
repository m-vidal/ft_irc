/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 09:19:53 by mvidal            #+#    #+#             */
/*   Updated: 2026/03/02 16:40:40 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(int ac, char **av)
{
	std::string password;
	unsigned short port;
	char c;

	if (ac != 3)
	{
		std::cerr << "Error: bad arguments.\n"
				  << "Usage: ./ircserver <port> <password>" << "\n";
		return 1;
	}

	std::stringstream ss(av[1]);
	if (!(ss >> port) || ss >> c)
		return (std::cerr << "Error: invalid port." << std::endl, 1);

	password = av[2];
	try
	{
		Server server(port, password);
		server.listenMode();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	return (0);
}
