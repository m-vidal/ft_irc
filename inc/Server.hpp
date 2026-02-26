/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 18:13:32 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/26 18:09:33 by atambo           ###   ########.fr       */
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
	void listenMode();

private:
	const short _port;
	const short _socket;
	const std::string _password;
	struct sockaddr_in _addr;
	std::list<Channel> _channels;
	std::map<unsigned short, Client> _clients;
	// ----------------------------------------
	void processMessage(int fd, std::string str);
	void sendToClient(int fd, std::string str);
	bool checkPassword(std::string password);
	void disconnectClient(int fd);
	User *getUser(std::string &nick);
	Channel *getChannel(std::string &nick);
};

#endif
