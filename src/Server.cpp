/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:33:23 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/15 23:11:48 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server(unsigned short &port, std::string &password): 
_password(password), _socket(socket(AF_INET, SOCK_STREAM, 0)), _port(port) {
	(void)_port;
	if (!checkPassword(password))
		throw std::runtime_error("Error: password too weak!");
	if (_socket < 0)
		throw std::runtime_error("Error: failure in the socket server creation!");

	int	opt = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("Error: failure in the socket server config!");	
	
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(_socket, reinterpret_cast<struct sockaddr *>(&_addr), sizeof(_addr)))
		throw std::runtime_error("Error: failure to associate the port.");
	is_running = false;
}

bool	Server::checkPassword(std::string password) {
	bool	hasUpper = false;
	bool	hasLower = false;
	bool	hasNumbr = false;
	bool	hasSmbl = false;
	
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


void	Server::listenMode() {
	if (listen(_socket, 5))
		throw std::runtime_error("Error: failure to enter listening mode!");
	
	int flag = fcntl(_socket, F_GETFL, 0);
	if (flag == -1 || fcntl(_socket, F_SETFL, flag | O_NONBLOCK) == -1)
		throw std::runtime_error("Errorn: failure to enter non block mode!");

	struct pollfd	server_fd;
	server_fd.fd = _socket;
	server_fd.events = POLLIN;
	server_fd.revents = 0;

	_polls.push_back(server_fd);
	is_running = true;
	while(is_running)
	{
		int poll_return = poll(_polls.data(), _polls.size(), 10000);
		if (poll_return < 0)
			throw std::runtime_error("Error: poll() sys call failed!");
		else if (poll_return == 0)
			continue ;
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
				char	buff[1024] = {0};
				size_t bytes_received = recv(_polls[i].fd, buff, sizeof(buff) - 1, 0);
				if (bytes_received > 0)
				{
					buff[bytes_received] = '\0';
					_users[_polls[i].fd].appendToBuffer(std::string(buff));
					std::string	aux = _users[_polls[i].fd].getBuffer();
					
					size_t	pos = aux.find("\\r\\n");
					if (pos != std::string::npos)
					{
						std::cout << "Client@" << _polls[i].fd << ": " << aux.substr(0, pos);
						aux.erase(0, pos + 4);
					}
				}
				else if (bytes_received == 0)
				{
					std::cout << "Client@" << _polls[i].fd << ": disconected " << std::endl;
					close(_polls[i].fd);
					_polls.erase(_polls.begin() + i);
					i--;
				}
				else
				{
					std::cerr << "Error receiving data from client@" << _polls[i].fd << std::endl;
					close(_polls[i].fd);
					_polls.erase(_polls.begin() + i); // Remover o cliente da lista
					--i;
				}
			}
		}
	}
	//processar linhas vindas do cliente
	//para cada linha completa instanciar o parser
}

Server::~Server(void) {
	if (_socket)
		close(_socket);
}
