/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 09:19:53 by mvidal            #+#    #+#             */
/*   Updated: 2026/01/27 10:59:26 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

    std::cout << "001" << std::endl;
    return (0);
    //std::cout << std::getenv("HOME") << std::endl;
    //std::cout << "Port number: " << port << "\nPassword: " << password << std::endl;
}
