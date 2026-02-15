/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:33:23 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/15 02:50:38 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server(unsigned short &port, std::string &password): 
_password(password), _socket(socket(AF_INET, SOCK_STREAM, 0)), _port(port) {
	(void)_port;
	if (_socket < 0)
		throw std::runtime_error("Error in socket server creation!");
	int	opt = 1;
	setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));	
	
	
	//ficar a espera de eventos
	//configurar comportamento quando há uma nova conexão
	//processar linhas vindas do cliente
	//para cada linha completa instanciar o parser
	is_running = true;
}

Server::~Server(void) {
	if (_socket)
		close(_socket);
}
