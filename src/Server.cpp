/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:33:23 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/14 20:07:52 by mvidal           ###   ########.fr       */
=======
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:33:23 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/14 14:45:35 by marcsilv         ###   ########.fr       */
>>>>>>> upstream/temp
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server(unsigned short &port, std::string &password): 
<<<<<<< HEAD
_password(password), _socket(socket(AF_INET, SOCK_STREAM, 0)), _port(port) {
	//configurar socket
	if (_socket < 0)
		throw std::runtime_error("Error in socket creation!");
	//"bind" socket
	
=======
_socket(socket(AF_INET, SOCK_STREAM, 0)),
_port(port),
_password(password) {

	//configurar socket
	//"bind" socket
>>>>>>> upstream/temp
	//ficar a espera de eventos
	//configurar comportamento quando há uma nova conexão
	//processar linhas vindas do cliente
	//para cada linha completa instanciar o parser
<<<<<<< HEAD
	is_running = true;
}

Server::~Server(void) {
	if (_socket)
		close(_socket);
=======

}

Server::~Server(void) {
	/*if (_socket)
		close(_socket);*/
>>>>>>> upstream/temp
}
