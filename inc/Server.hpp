/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 18:17:28 by atambo            #+#    #+#             */
/*   Updated: 2026/03/02 18:54:45 by atambo           ###   ########.fr       */
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

#include "User.hpp"
#include "Channel.hpp"
#include "Executer.hpp"

struct Client
{
	User user;
	std::string inbuf;
	std::string outbuf;
	int fd;
	size_t poll_idx; // Link to position in _polls vector
	bool is_auth;	 // Registration status
};

class Server
{
public:
	bool is_running;
	Server(unsigned short port, std::string password);
	~Server();

	void listenMode();
	// WRITER: Logic -> Buffer
	void ReplyClient(User &user, const std::string &msg);
	const std::string &getName() const;

private:
	const std::string _name;
	const short _port;
	const short _socket;
	const std::string _password;
	struct sockaddr_in _addr;
	std::vector<pollfd> _polls;
	Executer _executer;

	// Map uses FD as key for O(log N) lookup
	std::map<unsigned short, Client> _clients;

	/* --- Core Internal Logic --- */
	bool checkPassword(std::string password);
	void disconnectClient(const short fd);
	void syncPollIndices(); // Keeps poll_idx accurate after vector shifts

	/* --- Getters --- */
	User *getUser(std::string &nick);
	User *getUser(const short fd);
	Client *getClient(User &user);

	/* --- Data Flow: Reception (FD -> Command) --- */
	void handleEvents();
	void acceptNewClient();
	void readFdToBuffer(size_t &poll_idx); // Stage 1: Recv from socket
	void processBufferToCommand(int fd);   // Stage 2: Parse \r\n lines

	/* --- Data Flow: Transmission (Buffer -> FD) --- */
	void sendBufferToFd(int fd); // Stage 3: Send to socket
};

#endif