/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 09:19:53 by mvidal            #+#    #+#             */
/*   Updated: 2026/02/14 14:58:58 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <sstream>

#include "../inc/Server.hpp"

int main(int ac, char** av) {
    std::string		password;
    unsigned short	port;
    char			c;

	if (ac != 3)
        return (std::cerr << "Error: bad arguments." << std::endl, 1);

	std::stringstream ss(av[1]);
	if (!(ss >> port) || ss >> c) {
		return (std::cerr << "Error: invalid port." << std::endl, 1);
	}
	//task: verificar o range da password [1024;65536]
	std::cout << port << std::endl;
	password = av[2];
	try {
		Server server(port, password);
	} catch (std::runtime_error &e) {
		std::cout << e.what() << std::endl;
	}
	//task: verificar password vazia

	/*int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		return(std::cout << "Socket Not Created!" << std::endl, 1);
	}
	std::cout << "Socket fd: " << sockfd << std::endl;
	close(sockfd);*/
	return (0);
}
