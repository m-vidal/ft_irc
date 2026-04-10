/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 09:19:53 by mvidal            #+#    #+#             */
/*   Updated: 2026/04/01 13:13:17 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(int ac, char **av)
{
	std::string password;
	long long port_check = 0;
	unsigned short port;
	char c;

	if (ac != 3)
		return (std::cerr << "Error: bad arguments." << std::endl, 1);

	std::stringstream ss(av[1]);
	if (!(ss >> port_check) || ss.fail() || ss >> c  || port_check < PORT_MIN || port_check > PORT_MAX)
		return (std::cerr << "Error: invalid port.Port number must be between " << PORT_MIN << "-" << PORT_MAX << std::endl, 1);
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
		std::cerr << e.what() << std::endl;
	}

	return (0);
}
