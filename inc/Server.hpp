/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 18:13:32 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/25 19:47:46 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <fcntl.h>
#include <string>
#include <vector>
#include <list>
#include <map>
// ----------------------------------
#include "User.hpp"
#include "Channel.hpp"

typedef struct Client
{
	User user;
	std::string inbuf;
	std::string outbuf;
	pollfd poll; // para controlar os eventos de cada cliente
};

class Server
{

public:
	bool is_running;

	Server(unsigned short &port, std::string &password);
	Server operator=(const Server &other);
	Server(const Server &other);
	~Server();

	void listenMode();

private:
	void processMessage(int fd, std::string str);
	void sendToClient(int fd, std::string str);
	bool checkPassword(std::string password);
	void disconnectClient(int fd);

	std::list<Channel> _channels;
	const std::string _password;
	const short _socket;
	std::map<int, Client> _clients; // um mapa porque assim conseguimos identificar cada user pelo seu fd
	struct sockaddr_in _addr;
	const short _port;
};

#endif
