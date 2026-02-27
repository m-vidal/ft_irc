/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:05:41 by atambo            #+#    #+#             */
/*   Updated: 2026/02/27 15:54:07 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(unsigned short port, std::string password) : _port(port), _socket(socket(AF_INET, SOCK_STREAM, 0)), _password(password), _executer(*this)
{
	(void)_port;
	if (!checkPassword(password))
		throw std::runtime_error("Error: password too weak!");
	if (_socket < 0)
		throw std::runtime_error("Error: failure in the socket server creation!");

	int opt = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("Error: failure in the socket server config!");

	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(_socket, reinterpret_cast<struct sockaddr *>(&_addr), sizeof(_addr)))
		throw std::runtime_error("Error: failure to associate the port.");
	is_running = false;
}

bool Server::checkPassword(std::string password)
{
	bool hasUpper = false;
	bool hasLower = false;
	bool hasNumbr = false;
	bool hasSmbl = false;

	if (password.size() < 8)
		return (false);
	for (size_t i = 0; i < password.size(); i++)
	{
		if (std::isupper(password[i]))
			hasUpper = true;

		else if (std::islower(password[i]))
			hasLower = true;
		else if (std::isdigit(password[i]))
			hasNumbr = true;
		else
			hasSmbl = true;
	}
	if (hasUpper && hasLower && hasNumbr && hasSmbl)
		return (true);
	return (false);
}

// void Server::processMessage(int fd, std::string str)
// {
// 	// sendToClient(fd, str);
// 	// Parser parser(_users[fd], str);
// }

void Server::sendToClient(const short fd, std::string str)
{
	std::map<unsigned short, Client>::iterator it = _clients.find(fd);
	if (it == _clients.end())
		return;

	// Append the message (usually IRC needs \r\n at the end)
	it->second.outbuf += str;

	// Tell poll() we are now interested in WRITING to this socket
	for (size_t i = 0; i < _polls.size(); ++i)
	{
		if (_polls[i].fd == fd)
		{
			_polls[i].events |= POLLOUT;
			break;
		}
	}
}

void Server::disconnectClient(const short fd)
{
	// 1. Check if the client actually exists to avoid .at() crashes
	std::map<unsigned short, Client>::iterator it = _clients.find(fd);
	if (it == _clients.end())
		return;

	// 2. IMPORTANT: Clean up Channel references
	// Since User is stored BY VALUE inside Client, if you delete the Client,
	// any pointers to &it->second.user inside your Channels become DEAD.
	// for (std::list<Channel>::iterator cit = _channels.begin(); cit != _channels.end(); ++cit)
	// {
	// 	// You'll need a method in Channel to remove a user by pointer or nick
	// 	cit->removeMember(it->second.user.getName());
	// }

	// 3. Close the file descriptor
	std::cout << "Client " << fd << " (nick: " << it->second.user.getNick() << ") disconnected." << std::endl;
	close(fd);

	// 4. Remove from map (This cleans up the User and buffers automatically)
	_clients.erase(it);
}

void Server::listenMode()
{
	if (listen(_socket, 5) == -1)
		throw std::runtime_error("Error: listen failed");

	// Set server socket to non-blocking
	fcntl(_socket, F_SETFL, O_NONBLOCK);

	// Initial pollfd for the server itself
	pollfd server_pfd = {_socket, POLLIN, 0};
	_polls.push_back(server_pfd);
	is_running = true;

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
	// 1. Handle New Connections
	if (_polls[0].revents & POLLIN)
	{
		acceptNewClient();
	}

	// 2. Handle Existing Clients (Iterate backwards or carefully with i--)
	for (size_t i = 1; i < _polls.size(); ++i)
	{
		if (_polls[i].revents & POLLIN)
		{
			readFromClient(i);
		}
		else if (_polls[i].revents & (POLLERR | POLLHUP))
		{
			disconnectClient(_polls[i].fd);
			--i;
		}
	}
}

void Server::acceptNewClient()
{
	int client_fd = accept(_socket, NULL, NULL);
	if (client_fd == -1)
		return;

	fcntl(client_fd, F_SETFL, O_NONBLOCK);

	pollfd pfd = {client_fd, POLLIN, 0};
	_polls.push_back(pfd);

	// Create new Client entry in the map
	_clients.insert(std::make_pair(client_fd, Client()));
	std::cout << "Client " << client_fd << " connected." << std::endl;
}

void Server::readFromClient(size_t &poll_idx)
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
	processBuffer(fd);
}

void Server::processBuffer(int fd)
{
	std::string &buf = _clients[fd].inbuf;
	size_t pos;

	while ((pos = buf.find("\r\n")) != std::string::npos)
	{
		std::string message = buf.substr(0, pos);
		buf.erase(0, pos + 2);

		if (!message.empty())
		{
			_executer.processMessage(&(_clients[fd].user), message);
		}
	}
}

Server::~Server()
{
	if (_socket)
		close(_socket);
}

User *Server::getUser(std::string &nick)
{
	std::map<unsigned short, Client>::iterator it = _clients.begin();
	while (it != _clients.end())
	{
		if (it->second.user.getNick() == nick)
			return &(it->second.user);
		it++;
	}
	return NULL;
}

User *Server::getUser(const short fd)
{
	std::map<unsigned short, Client>::iterator it = _clients.find(fd);
	if (it->first == fd)
		return &(it->second.user);
	return NULL;
}

// Channel *Server::getChannel(std::string &name)
// {
// 	std::list<Channel>::iterator it = _channels.begin();
// 	while (it != _channels.end())
// 	{
// 		if (it->getName() == name)
// 			return &(*it);
// 		it++;
// 	}
// 	return NULL;
// }
