/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:33:23 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/14 14:45:35 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server(unsigned short &port, std::string &password): 
_socket(socket(AF_INET, SOCK_STREAM, 0)),
_port(port),
_password(password) {

	//configurar socket
	//"bind" socket
	//ficar a espera de eventos
	//configurar comportamento quando há uma nova conexão
	//processar linhas vindas do cliente
	//para cada linha completa instanciar o parser

}

Server::~Server(void) {
	/*if (_socket)
		close(_socket);*/
}
