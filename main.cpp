/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 09:19:53 by mvidal            #+#    #+#             */
/*   Updated: 2026/01/30 16:20:52 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <sstream>

int main(int ac, char** av)
{
    std::string     password;
    unsigned short  port;
    char            c;

    if (ac !=3)
        return (std::cerr << "Error: bad arguments." << std::endl, 1);

    std::stringstream ss(av[1]);
    if (!(ss >> port) || ss >> c)
        return (std::cerr << "Error: invalid port." << std::endl, 1);

    password = av[2];
    if (password != std::getenv("PASSWORD"))
        return (std::cerr << "Error: wrong password." << std::endl, 1);

    {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            return(std::cout << "Socket Not Created!" << std::endl, 1);
        }
        std::cout << "Socket fd: " << sockfd << std::endl;
        close(sockfd);
    }
    return (0);
}
