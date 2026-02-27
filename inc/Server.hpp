/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 18:13:32 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/27 15:55:16 by atambo           ###   ########.fr       */
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

struct Client
{
	User user;
	std::string inbuf;
	std::string outbuf;
};

class Server
{
public:
	bool is_running;
	Server(unsigned short port, std::string password);
	~Server();
	void listenMode();

private:
	const short _port;
	const short _socket;
	const std::string _password;
	struct sockaddr_in _addr;
	std::vector<pollfd> _polls;
	Executer _executer;
	// ----------------------------------------
	std::map<unsigned short, Client> _clients;
	// std::list<Channel> _channels;
	// ----------------------------------------
	void processMessage(const short fd, std::string str);
	void sendToClient(const short fd, std::string str);
	bool checkPassword(std::string password);
	void disconnectClient(const short fd);
	User *getUser(std::string &nick);
	User *getUser(const short fd);
	// Channel *getChannel(std::string &nick);
	// listen mode helpers --------------------
	void handleEvents();
	void acceptNewClient();
	void readFromClient(size_t &poll_idx);
	void processBuffer(int fd);
};

#endif
