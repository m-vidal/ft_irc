/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 18:16:49 by atambo            #+#    #+#             */
/*   Updated: 2026/03/02 18:52:50 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* --- Construction & Setup --- */

Server::Server(unsigned short port, std::string password)
	: _name("ft_ircserver"), _port(port), _socket(socket(AF_INET, SOCK_STREAM, 0)),
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

	is_running = false;
}

const std::string &Server::getName() const { return _name; }

Server::~Server()
{
	if (_socket)
		close(_socket);
}

/* --- Core Loop & Event Handling --- */

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
		if (poll(_polls.data(), _polls.size(), -1) < 0)
		{
			if (is_running)
				throw std::runtime_error("Error: poll failed");
			break;
		}
		handleEvents();
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
			readFdToBuffer(i);

		// WRITE: Buffer -> FD
		if (i < _polls.size() && (_polls[i].revents & POLLOUT))
			sendBufferToFd(_polls[i].fd);

		// ERRORS/HUP
		if (i < _polls.size() && (_polls[i].revents & (POLLERR | POLLHUP)))
		{
			disconnectClient(_polls[i].fd);
			--i;
		}
	}
}

/* --- Data Flow: Reception --- */

void Server::acceptNewClient()
{
	int client_fd = accept(_socket, NULL, NULL);
	if (client_fd == -1)
		return;

	fcntl(client_fd, F_SETFL, O_NONBLOCK);

	pollfd pfd = {client_fd, POLLIN, 0};
	_polls.push_back(pfd);

	Client new_client;
	new_client.fd = client_fd;
	new_client.is_auth = false;
	new_client.poll_idx = _polls.size() - 1;

	_clients.insert(std::make_pair(client_fd, new_client));
	std::cout << "Client FD " << client_fd << " connected at index " << new_client.poll_idx << std::endl;
}

void Server::readFdToBuffer(size_t &poll_idx)
{
	char buffer[1024];
	int fd = _polls[poll_idx].fd;
	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);

	if (bytes <= 0)
	{
		disconnectClient(fd);
		--poll_idx;
		return;
	}

	buffer[bytes] = '\0';
	_clients[fd].inbuf += buffer;

	// Move to next stage: Buffer -> String/Command
	processBufferToCommand(fd);
}

void Server::processBufferToCommand(int fd)
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
void Server::sendBufferToFd(int fd)
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

void Server::disconnectClient(const short fd)
{
	std::map<unsigned short, Client>::iterator it = _clients.find(fd);
	if (it == _clients.end())
		return;

	std::cout << "Client FD " << fd << " disconnected." << std::endl;

	// Remove from poll vector
	for (size_t i = 0; i < _polls.size(); ++i)
	{
		if (_polls[i].fd == fd)
		{
			_polls.erase(_polls.begin() + i);
			break;
		}
	}

	close(fd);
	_clients.erase(it);

	// Re-sync all indices because the vector shifted
	syncPollIndices();
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