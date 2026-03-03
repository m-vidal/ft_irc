/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 18:17:28 by atambo            #+#    #+#             */
/*   Updated: 2026/03/03 11:49:46 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#define TIMEOUT_LIMIT 5
#define AUTHENTICAT_TIME 10
#define DISCONNECT_TIME 15

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <exception>
#include <fcntl.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>

#include "User.hpp"
#include "Channel.hpp"
#include "Executer.hpp"

struct Client
{
	int fd;
	uint32_t ip;
	std::string ip_str;
	bool is_auth;
	bool ping_sent;
	std::string inbuf;
	std::string outbuf;
	User user;
	time_t last_active;
	time_t connected_at;
	size_t poll_idx;
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
	std::vector<int> _to_disconnect;

	Executer _executer;
	// Map uses FD as key for O(log N) lookup
	std::map<unsigned short, Client> _clients;

	/* --- Core Internal Logic --- */
	void pingClients();
	bool checkPassword(std::string password);
	void markForDisconnect(const short fd, const std::string &reason);
	void syncPollIndices(); // Keeps poll_idx accurate after vector shifts

	/* --- Getters --- */
	User *getUser(std::string &nick);
	User *getUser(const short fd);
	Client *getClient(User &user);

	/* --- Data Flow: Reception (FD -> Command) --- */
	void handleEvents();
	void acceptNewClient();
	void fdToBuffer(size_t &poll_idx); // Stage 1: Recv from socket
	void bufferToCommand(int fd);	   // Stage 2: Parse \r\n lines

	/* --- Data Flow: Transmission (Buffer -> FD) --- */
	void sendBuffer(int fd); // Stage 3: Send to socket
	void processDisconnects();
};

#endif