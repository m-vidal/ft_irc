/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:33:23 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/15 11:39:00 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server(unsigned short &port, std::string &password): 
_password(password), _socket(socket(AF_INET, SOCK_STREAM, 0)), _port(port) {
	(void)_port;
	if (_socket < 0)
		throw std::runtime_error("Error failure in the socket server creation!");

	int	opt = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("Error: failure in the socket server config!");	
	
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(_socket, reinterpret_cast<struct sockaddr *>(&_addr), sizeof(_addr)))
		throw std::runtime_error("Error: failure to associate the port.");
	is_running = false;
}

void	Server::listenMode() {
	if (listen(_socket, 5))
		throw std::runtime_error("Error: failure to enter listening mode!");
	
	int flag = fcntl(_socket, F_GETFL, 0);
	if (flag == -1 || fcntl(_socket, F_SETFL, flag | O_NONBLOCK) == -1)
		throw std::runtime_error("Errorn: failure to enter non block mode!");
	is_running = true;
	
	while(is_running) {
		usleep(100000);
	}
	//configurar comportamento quando há uma nova conexão
	//processar linhas vindas do cliente
	//para cada linha completa instanciar o parser
}

Server::~Server(void) {
	if (_socket)
		close(_socket);
}
