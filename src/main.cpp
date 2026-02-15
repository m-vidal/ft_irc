/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 09:19:53 by mvidal            #+#    #+#             */
/*   Updated: 2026/02/15 13:55:53 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>
#include <sys/types.h>
#include <iostream>
#include <cstdlib>

#include "../inc/Server.hpp"

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
		Server server(port, password);
		
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}

	return (0);
}
