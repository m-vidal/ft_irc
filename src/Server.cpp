/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:05:41 by atambo            #+#    #+#             */
/*   Updated: 2026/02/26 19:13:21 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(unsigned short port, std::string password) : _port(port), _socket(socket(AF_INET, SOCK_STREAM, 0)), _password(password)
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

void Server::sendToClient(int fd, std::string str)
{
	const char *aux = str.c_str();
	size_t size = str.size();
	size_t total_read = 0;

	while (total_read < size)
	{
		size_t sent_bytes = send(fd, aux + total_read, size - total_read, 0);
		if (sent_bytes < 1)
		{
			std::cout << "Client@" << fd << ": " << "disconnected" << std::endl;
			return;
		}
		total_read += sent_bytes;
	}
}

void Server::disconnectClient(const short fd)
{
	Client &client = _clients.at(fd);
	for (size_t i = 0; i < _polls.size(); i++)
	{
		if (_polls[i].fd == fd)
		{
			_polls.erase(_polls.begin() + i);
			break;
		}
	}
	_users.erase(fd);
	close(fd);
}

void Server::listenMode()
{
	if (listen(_socket, 5))
		throw std::runtime_error("Error: failure to enter listening mode!");

	int flag = fcntl(_socket, F_GETFL, 0);
	if (flag == -1 || fcntl(_socket, F_SETFL, flag | O_NONBLOCK) == -1)
		throw std::runtime_error("Errorn: failure to enter non block mode!");

	struct pollfd server_fd;
	server_fd.fd = _socket;
	server_fd.events = POLLIN;
	server_fd.revents = 0;

	_polls.push_back(server_fd);
	is_running = true;
	while (is_running)
	{
		int poll_return = poll(_polls.data(), _polls.size(), 10000);
		if (poll_return < 0)
			throw std::runtime_error("Error: poll() sys call failed!");
		else if (poll_return == 0)
			continue;
		if (_polls[0].revents & POLLIN)
		{
			int clientfd = accept(_socket, NULL, NULL);
			if (clientfd > -1)
			{
				std::cout << clientfd << " Conected" << std::endl;
				int client_flag = fcntl(clientfd, F_GETFL);
				fcntl(clientfd, F_SETFL, client_flag | O_NONBLOCK);

				struct pollfd client;
				client.fd = clientfd;
				client.events = POLLIN;
				client.revents = 0;

				_polls.push_back(client);
				_users.insert(std::make_pair(static_cast<int>(clientfd), User()));
			}
		}
		for (size_t i = 1; i < _polls.size(); ++i)
		{
			if (_polls[i].revents & POLLIN)
			{
				char buff[1024] = {0};
				size_t bytes_received = recv(_polls[i].fd, buff, sizeof(buff) - 1, 0);
				if (bytes_received > 0)
				{
					buff[bytes_received] = '\0';
					_users[_polls[i].fd].appendToBuffer(std::string(buff));
					std::string aux = _users[_polls[i].fd].getBuffer();

					size_t pos;
					while ((pos = aux.find("\r\n")) != std::string::npos)
					{
						std::string line(aux.substr(0, pos));
						processMessage(_polls[i].fd, line);
						_users[_polls[i].fd].clearBuffer(pos + 2);
						aux.erase(0, pos + 2);
					}
				}
				else if (bytes_received == 0)
				{
					std::cout << "Client@" << _polls[i].fd << ": disconected " << std::endl;
					_users[_polls[i].fd].clearBuffer(_users[_polls[i].fd].getBuffer().size());
					disconnectClient(_polls[i].fd);
					i--;
				}
				else
				{
					std::cerr << "Error receiving data from client@" << _polls[i].fd << std::endl;
					disconnectClient(_polls[i].fd);
					--i;
				}
			}
		}
	}
}

Server::~Server(void)
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

Channel *Server::getChannel(std::string &name)
{
	std::list<Channel>::iterator it = _channels.begin();
	while (it != _channels.end())
	{
		if (it->getName() == name)
			return &(*it);
		it++;
	}
	return NULL;
}
