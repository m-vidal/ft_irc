/* ************************************************************************** */
/*	                                                                        */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:33:23 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/07 03:16:18 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server(unsigned short &port, std::string &password): 
_password(password), _socket(socket(AF_INET, SOCK_STREAM, 0)), _port(port) {
	(void)_port;
	if (_password.empty())
		throw std::runtime_error("Error: empty password!");
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
	setknowscommands();
	_received_sig = false;
	is_running = false;
}


void	Server::setknowscommands()
{
	_commands["PRIVMSG"] = &Server::msg;
    _commands["QUIT"] = &Server::quit;
}

void Server::autUser(int fd, std::string str)
{
	std::stringstream ss(str);
	std::string command[2];

	ss >> command[0] >> command[1];
	if (command[0] == "PASS" && command[1] != _password)
		sendToClient(fd, ":ircserv 464 * :Password incorrect\r\n");
	else if (command[0] == "PASS" && command[1] == _password)
		_users.at(fd).setStepOne();
	else if (_users.at(fd).getStepOne())
	{
		if (command[0] == "NICK")
		{
			if (getFdFromNick(command[1]) == -1 || getFdFromNick(command[1]) == fd)
				_users.at(fd).setNick(command[1]);
			else 
				sendToClient(fd, ":ircserv 433 * NickJaExiste :Nickname is already in use\r\n");
		}
		if (command[0] == "USER" && command[1].size())
			_users.at(fd).setUser(command[1]);
		if (_users.at(fd).getUsername() != "default" && _users.at(fd).getNick() != "default")
		{
			sendToClient(fd, ":ircsRev 001 * :Welcome to the The Box IRC\r\n");
			_users.at(fd).authenticate();
		}
	}
}



void	Server::quit(int fd, std::vector<std::string>& params, std::string trailing)
{
	(void)params;
	if (trailing.size())
	{
		std::string msg = ":" + _users[fd].getNick() + "!" + _users[fd].getUsername();
		msg += "@127.0.0.1 QUIT : " + trailing + "\r\n";
		sendToClient(fd, msg);
	}
	else
		std::cout << "User: " + _users[fd].getNick() + " disconnected" << std::endl;
	disconnectClient(fd);
}

void	Server::msg(int fd, std::vector<std::string>& params, std::string trailing)
{
	std::string prefix = ":" + _users[fd].getNick() + "!" + _users[fd].getUsername() + "@" + "127.0.0.1";
	std::string forward = prefix + " PRIVMSG " + params[0] + " " + trailing + "\r\n";
	if (!params.size() || !trailing.size())
	{
		sendToClient(fd, ":ircserv 461 PRIVMSG :Not enough parameters\r\n");
		return ;
	}

	int destin = getFdFromNick(params[0]);
	if (destin > 0)
		sendToClient(destin, forward);
}


int	Server::getFdFromNick(std::string nick)
{
	for (size_t i = 1; i < _polls.size(); i++)
	{
		if (_users[_polls[i].fd].getNick() != nick)
			continue ;
		return (_users[_polls[i].fd].getFd());
	}
	return (-1);
}


std::vector<std::string> split(const std::string& input) {
	std::vector<std::string> result;
	std::string token;
	std::string::size_type i = 0;
	const std::string::size_type n = input.size();

	while (i < n && (input[i] == ' ' || input[i] == '\t'))
		++i;

	while (i < n) {
		token.clear();
		while (i < n && input[i] != ' ' && input[i] != '\t') {
			token += input[i];
			++i;
		}

		if (!token.empty())
			result.push_back(token);
		while (i < n && (input[i] == ' ' || input[i] == '\t'))
			++i;
	}
	return (result);
}


void Server::parser(User &user, std::string &str) {
	//no caso de faltar args ou trailing, mandar args vazios
	std::string trimmed;
	if (str.size() >= 2)
		trimmed = str.substr(0, str.find("\r\n")); //trimming the \r\n

	std::size_t commaPos = trimmed.find(":");
	std::string	trailing;

	if (commaPos != std::string::npos){
		trailing = trimmed.substr(commaPos + 1); //verificar mais tarde
	}

	std::string	strBeforeTrailing = trimmed.substr(0, commaPos); //COMMAND param1 param2 ....

	std::string					command;

	std::size_t spacePos = strBeforeTrailing.find(' ');

	command = strBeforeTrailing.substr(0, spacePos);
	std::size_t commandLength = command.length();
	for (std::size_t i = 0; i < commandLength; i++) {
		command[i] = std::toupper(command[i]);
	}
	std::string tempArgs = strBeforeTrailing.substr(spacePos + 1);
	std::vector<std::string> args = split(tempArgs);

	std::map<std::string, CommandFunc>::iterator it = _commands.find(command);
	if (it != _commands.end())
		(this->*(it->second))(user.getFd(), args, trailing);
}



void	Server::processMessage(int fd, std::string str)
{
	if (!_users.at(fd).isAuthenticated())
	{
		autUser(fd, str);
	}
	else
		this->parser(_users[fd], str);
}


void	Server::sendToClient(int fd, std::string str)
{
	send(fd, str.c_str(), str.size(), 0);
}


void	Server::disconnectClient(int fd) {
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


void	Server::listenMode() {
	if (listen(_socket, 5))
		throw std::runtime_error("Error: failure to enter listening mode!");

	if (fcntl(_socket, F_SETFL, O_NONBLOCK) == -1)
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
		if (_received_sig || poll_return == 0)
			continue ;
		else if (poll_return < 0)
			throw std::runtime_error("Error: poll() sys call failed!");
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
				_users.insert(std::make_pair(static_cast<int>(clientfd), User(clientfd)));
			
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

					size_t	pos;
					while ((pos = aux.find("\r\n")) != std::string::npos)
					{
						std::string	line(aux.substr(0, pos));
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


void	Server::turnOff() { _received_sig = true; is_running = false; }

Server::~Server(void) {
	if (_socket)
		close(_socket);
}
