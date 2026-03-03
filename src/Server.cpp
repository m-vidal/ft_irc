/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 18:16:49 by atambo            #+#    #+#             */
/*   Updated: 2026/03/03 11:53:48 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* --- Construction & Setup --- */

Server::Server(unsigned short port, std::string password)
	: _name("ircserv"), _port(port), _socket(socket(AF_INET, SOCK_STREAM, 0)),
	  _password(password), _executer(*this)
{
	if (!checkPassword(password))
		throw std::runtime_error("Error: password too weak!");
	if (_socket < 0)
		throw std::runtime_error("Error: socket creation failed!");

	int opt = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("Error: setsockopt failed!");

	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(_socket, reinterpret_cast<struct sockaddr *>(&_addr), sizeof(_addr)))
		throw std::runtime_error("Error: bind failed!");

	const unsigned int timeout_limit = TIMEOUT_LIMIT;
	const unsigned int dc_limit = DISCONNECT_TIME;
	if (dc_limit <= timeout_limit)
		throw std::invalid_argument("Bad time limit values : disconnect time must be greater than timeout_limit");

	is_running = false;
}

const std::string &Server::getName() const { return _name; }

Server::~Server()
{
	if (_socket)
		close(_socket);
}

/* --- Core Loop & Event Handling --- */

void Server::pingClients()
{
	time_t now = time(NULL);
	const unsigned int timeout_limit = TIMEOUT_LIMIT;
	const unsigned int dc_limit = DISCONNECT_TIME;
	const unsigned int auth_limit = AUTHENTICAT_TIME;

	std::vector<int> to_disconnect;

	for (std::map<unsigned short, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		// --- 1. AUTHENTICATION TIMEOUT ---
		// If they aren't auth'd and have been connected longer than auth_limit
		if (!it->second.is_auth && difftime(now, it->second.connected_at) > auth_limit)
		{
			std::cout << "Auth timeout for FD " << it->first << std::endl;
			to_disconnect.push_back(it->first);
			continue; // Skip further checks for this client
		}

		double diff = difftime(now, it->second.last_active);

		// --- 2. GHOST DISCONNECT ---
		if (diff > dc_limit)
		{
			to_disconnect.push_back(it->first);
		}
		// --- 3. PING NUDGE ---
		else if (diff > timeout_limit && !it->second.ping_sent)
		{
			std::string ping_msg = "PING :" + _name + "\r\n";
			it->second.outbuf += ping_msg;
			_polls[it->second.poll_idx].events |= POLLOUT;
			it->second.ping_sent = true;
		}
	}

	for (size_t i = 0; i < to_disconnect.size(); ++i)
		markForDisconnect(to_disconnect[i], "Connection timed out");
}

void Server::processDisconnects()
{
	if (_to_disconnect.empty())
		return;

	for (size_t i = 0; i < _to_disconnect.size(); ++i)
	{
		int fd = _to_disconnect[i];
		std::map<unsigned short, Client>::iterator it = _clients.find(fd);

		if (it == _clients.end())
			continue;

		// 1. Remove from the poll vector
		for (std::vector<pollfd>::iterator p_it = _polls.begin(); p_it != _polls.end(); ++p_it)
		{
			if (p_it->fd == fd)
			{
				_polls.erase(p_it);
				break;
			}
		}

		// 2. Close the socket and erase the client
		close(fd);
		_clients.erase(it);

		std::cout << "Successfully cleaned up FD " << fd << std::endl;
	}

	// 3. Clear the queue
	_to_disconnect.clear();

	// 4. Re-sync indices because the vector shifted
	syncPollIndices();
}

void Server::listenMode()
{
	if (listen(_socket, 5) == -1)
		throw std::runtime_error("Error: listen failed");

	fcntl(_socket, F_SETFL, O_NONBLOCK);

	pollfd server_pfd = {_socket, POLLIN, 0};
	_polls.push_back(server_pfd);

	is_running = true;
	std::cout << "IRC Server running on port " << _port << "..." << std::endl;

	while (is_running)
	{
		pingClients();
		if (poll(_polls.data(), _polls.size(), 1000) < 0)
		{
			if (is_running)
				throw std::runtime_error("Error: poll failed");
			break;
		}
		handleEvents();
		processDisconnects();
	}
}

void Server::handleEvents()
{
	// 1. New Connections
	if (_polls[0].revents & POLLIN)
		acceptNewClient();

	// 2. Existing Clients
	for (size_t i = 1; i < _polls.size(); ++i)
	{
		// READ: FD -> Buffer
		if (_polls[i].revents & POLLIN)
			fdToBuffer(i);

		// WRITE: Buffer -> FD
		if (i < _polls.size() && (_polls[i].revents & POLLOUT))
			sendBuffer(_polls[i].fd);

		// ERRORS/HUP
		if (i < _polls.size() && (_polls[i].revents & (POLLERR | POLLHUP)))
		{
			markForDisconnect(_polls[i].fd, "Not sure what this error msh should be");
			--i;
		}
	}
}

/* --- Data Flow: Reception --- */

void Server::acceptNewClient()
{
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);

	// Capture the client's address info
	int client_fd = accept(_socket, (struct sockaddr *)&client_addr, &addr_len);
	if (client_fd == -1)
		return;

	fcntl(client_fd, F_SETFL, O_NONBLOCK);

	pollfd pfd = {client_fd, POLLIN, 0};
	_polls.push_back(pfd);

	Client new_client;
	new_client.fd = client_fd;
	new_client.is_auth = false;
	new_client.poll_idx = _polls.size() - 1;
	new_client.last_active = time(NULL);
	new_client.ping_sent = false;
	new_client.connected_at = time(NULL);

	// Store the raw 32-bit integer IP
	new_client.ip = client_addr.sin_addr.s_addr;

	_clients.insert(std::make_pair(client_fd, new_client));

	// Optional: Log the IP immediately for debugging
	std::cout << "Client FD " << client_fd << " ("
			  << inet_ntoa(client_addr.sin_addr)
			  << ") connected at poll index " << new_client.poll_idx << std::endl;
}

void Server::fdToBuffer(size_t &poll_idx)
{
	char buffer[1024];
	int fd = _polls[poll_idx].fd;
	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);

	if (bytes <= 0)
	{
		markForDisconnect(fd, "Client ended connection");
		--poll_idx;
		return;
	}

	buffer[bytes] = '\0';
	_clients[fd].inbuf += buffer;
	_clients[fd].last_active = time(NULL);
	_clients[fd].ping_sent = false;

	// Move to next stage: Buffer -> String/Command
	bufferToCommand(fd);
}

void Server::bufferToCommand(int fd)
{
	std::string &buf = _clients[fd].inbuf;
	size_t pos;

	while ((pos = buf.find("\r\n")) != std::string::npos)
	{
		std::string message = buf.substr(0, pos);
		buf.erase(0, pos + 2);

		if (!message.empty())
			_executer.processMessage(&(_clients[fd].user), message);
	}
}

/* --- Data Flow: Transmission --- */

// WRITER: Command -> Buffer
void Server::ReplyClient(User &user, const std::string &msg)
{
	Client *client = this->getClient(user);
	if (!client)
		return;

	client->outbuf += msg;
	// Enable the bit so poll() knows we have something to send
	_polls[client->poll_idx].events |= POLLOUT;
}

// SENDER: Buffer -> FD
void Server::sendBuffer(int fd)
{
	Client &c = _clients[fd];
	if (c.outbuf.empty())
		return;

	ssize_t sent = send(fd, c.outbuf.c_str(), c.outbuf.size(), 0);

	if (sent > 0)
		c.outbuf.erase(0, sent);

	// If everything is sent, stop monitoring POLLOUT to save CPU
	if (c.outbuf.empty())
		_polls[c.poll_idx].events &= ~POLLOUT;
}

/* --- Utilities & Cleanup --- */

void Server::markForDisconnect(const short fd, const std::string &reason)
{
	// 1. Send the ERROR message immediately so the client knows why
	std::string msg = Reply::err_disconnect(_clients[fd].ip_str, reason);
	send(fd, msg.c_str(), msg.length(), 0);

	// 2. Add to the list if not already there
	if (std::find(_to_disconnect.begin(), _to_disconnect.end(), fd) == _to_disconnect.end())
		_to_disconnect.push_back(fd);
}

void Server::syncPollIndices()
{
	for (size_t i = 1; i < _polls.size(); ++i)
	{
		int fd = _polls[i].fd;
		_clients[fd].poll_idx = i;
	}
}

Client *Server::getClient(User &target)
{
	std::map<unsigned short, Client>::iterator it = _clients.begin();
	for (; it != _clients.end(); ++it)
	{
		if (&(it->second.user) == &target)
			return &(it->second);
	}
	return NULL;
}

bool Server::checkPassword(std::string password)
{
	if (password.size() < 8)
		return false;
	bool up = false, low = false, num = false, symb = false;
	for (size_t i = 0; i < password.size(); i++)
	{
		if (isupper(password[i]))
			up = true;
		else if (islower(password[i]))
			low = true;
		else if (isdigit(password[i]))
			num = true;
		else
			symb = true;
	}
	return (up && low && num && symb);
}