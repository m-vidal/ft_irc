/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:33:23 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/28 21:33:15 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include <sys/socket.h>

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
	setknowncommands();
	is_running = false;
}

void	Server::setknowncommands() {
	// _commands["PRIVMSG"] = &Server::msg;
	_commands["PASS"] = &Server::pass;
	_commands["NICK"] = &Server::nick;
	_commands["USER"] = &Server::user;
}

void	Server::user(int fd, std::vector<std::string>& params, std::string trailing) {
	if (_users[fd].isAuthenticated() == true) {
		ircReply(fd, ERR_ALREADYREGISTERED, "USER", "User already registered!");
		return ;
	}
	if (params.size() < 3 || trailing.empty() == true) {
		ircReply(fd, ERR_NEEDMOREPARAMS, "USER", "Not enough parameters!");
		return ;
	}
	_users[fd].setUser(params[0]);
	_users[fd].setHostname(params[1]);
	_users[fd].setRealname(trailing);
	if (_users[fd].checkIsPassAccepted() == true || _users[fd].checkIsUserSet() == true || _users[fd].checkIsNickSet() == true) {
		_users[fd].setIsAuthenticated();
		std::cout << "User: " << _users[fd].getNick() << " is is authenticated." << std::endl;
	}
}

User	*Server::findUserByNick(const std::string& nick) {
	for (size_t i = 1; i < _polls.size(); ++i) {
        int fd = _polls[i].fd;

        // opcional: ignorar o fd do próprio servidor se estiver no índice 0
        if (_users.find(fd) == _users.end())
            continue ;

        if (_users[fd].getNick() == nick)
            return (&_users[fd]);
	}
    return (NULL);
}

void	Server::nick(int fd, std::vector<std::string>& params, std::string trailing) {
	(void)trailing;
	if(params.size() == 0) {
		ircReply(fd, ERR_NONICKNAMEGIVEN, "NICK", "No nickname given!");
		return ;
	}
	if (std::isdigit(params[0][0]) || params[0].find_first_of(" ,*!@:#\n")){
		ircReply(fd, ERR_ERRONEUSNICKNAME, "NICK", "Invalid nickname!");
		return ;
	}
	if (findUserByNick(_users[fd].getNick()) != NULL) {
		ircReply(fd, ERR_NICKNAMEINUSE, "NICK", "Nickname already in use!");
		return ;
	}
	if (_users[fd].isAuthenticated() == true)
		ircReply(fd, "NICK", params[0]);
	_users[fd].setNick(params[0]);
	if (_users[fd].checkIsPassAccepted() == true || _users[fd].checkIsUserSet() == true || _users[fd].checkIsNickSet() == true) {
		_users[fd].setIsAuthenticated();
		std::cout << "User: " << _users[fd].getNick() << " is is authenticated." << std::endl;
	}
}
std::string Server::getUserNick(int fd) const {
	if (_users.at(fd).isAuthenticated() == false) {
		return "*";
	}
	return (_users.at(fd).getNick());
}

void	Server::pass(int fd, std::vector<std::string>& params, std::string trailing) {
	if(_users[fd].isAuthenticated()) {
		ircReply(fd, ERR_ALREADYREGISTERED, "PASS", "User already registered!");
		return ;
	}
	if (params.size() == 0) {
		ircReply(fd, ERR_NEEDMOREPARAMS, "PASS", trailing);
		return ;
	}
	if (params[0] != _password) {
		ircReply(fd, ERR_PASSWDMISMATCH, "PASS", "Wrong password!");
		return ;
	}
	_users[fd].setPassAccepted();//sets passAccepted to true
}

void	Server::msg(int fd, std::vector<std::string>& params, std::string trailing) {
	std::string prefix = ":" + _users[fd].getNick() + "!" + _users[fd].getUsername() + "@" + "127.0.0.1";
	std::string forward = prefix + " PRIVMSG " + params[0] + " " + trailing + "\r\n";
	if (!params.size() || !trailing.size()) {
		sendToClient(fd, ":ircserv 461 PRIVMSG :Not enough parameters\r\n");
        return ;
	}

	int destin = getFdFromNick(params[0]);
	if (destin > 0)
		sendToClient(destin, forward);
}

int	Server::getFdFromNick(std::string nick) {
	for (size_t i = 1; i < _polls.size(); i++) {
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

bool	Server::checkPassword(std::string password) {
	bool	hasUpper = false;
	bool	hasLower = false;
	bool	hasNumbr = false;
	bool	hasSmbl = false;
	
	if (password.size() < 8)
		return (false);
	for (size_t i = 0; i < password.size(); i++) {
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

void	Server::processMessage(int fd, std::string str) {
	// if (!_users.at(fd).isAuthenticated()) {
	// 	authUser(fd, str);
	// }
	// else
		this->parser(_users[fd], str);
}
//:ircserv 000 marcsilv NICK :traukubg
//:prefix 123 [params1][param2][params3] :trailing
void	Server::ircReply(int fd, int code, const std::string &command, const std::string &trailing) {
    std::stringstream ss;

    ss << std::setw(3) << std::setfill('0') << code;
	std::string reply = ":ircserv " + ss.str() + " " + getUserNick(fd) + " " + command + " :" + trailing + "\r\n";
	send(fd, reply.c_str(), reply.size(), 0);
}

void	Server::ircReply(int fd, const std::string &command, const std::string &trailing) {
	std::string reply = ":" + _users[fd].getNick() +
						"!" + _users[fd].getUsername() +
						"@" + _users[fd].getHostname() +
						" " + command + " :" + trailing + "\r\n";
	send(fd, reply.c_str(), reply.size(), 0);
}

void	Server::sendToClient(int fd, std::string str) {
	send(fd, str.c_str(), str.size(), 0);
}

void	Server::disconnectClient(int fd) {
	for (size_t i = 0; i < _polls.size(); i++) {
		if (_polls[i].fd == fd) {
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
		throw std::runtime_error("Error: failure to enter non block mode!");

	struct pollfd	server_fd;
	server_fd.fd = _socket;
	server_fd.events = POLLIN;
	server_fd.revents = 0;

	_polls.push_back(server_fd);
	is_running = true;
	std::cout << "Listening on port: " << _port << std::endl;
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
				std::cout << "A client with fd:" << clientfd << " has connected" << std::endl;
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
		for (size_t i = 1; i < _polls.size(); ++i) {
			if (_polls[i].revents & POLLIN) {
				char	buff[1024] = {0};
				size_t bytes_received = recv(_polls[i].fd, buff, sizeof(buff) - 1, 0);
				if (bytes_received > 0) {
					buff[bytes_received] = '\0';
					_users[_polls[i].fd].appendToBuffer(std::string(buff));
					std::string	aux = _users[_polls[i].fd].getBuffer();
					
					size_t	pos;
					while ((pos = aux.find("\r\n")) != std::string::npos) {
						std::string	line(aux.substr(0, pos));
						processMessage(_polls[i].fd, line);
						_users[_polls[i].fd].clearBuffer(pos + 2);
						aux.erase(0, pos + 2);
					}
				}
				else if (bytes_received == 0) {
					std::cout << "Client@" << _users[_polls[i].fd].getNick() << ": disconnected " << std::endl;
					_users[_polls[i].fd].clearBuffer(_users[_polls[i].fd].getBuffer().size());
					disconnectClient(_polls[i].fd);
					i--;
				}
				else {
					std::cerr << "Error receiving data from client@" << _polls[i].fd << std::endl;
					disconnectClient(_polls[i].fd);
					--i;
				}
			}
		}
	}
}

Server::~Server(void) {
	if (_socket)
		close(_socket);
}

/*

void Server::handlePrivmsg(int fd, std::vector<std::string>& params)
{
    if (params.empty() || params.size() < 2)
    {
        sendToClient(fd, ":ircserv 461 PRIVMSG :Not enough parameters\r\n");
        return ;
    }

    std::string destination = params[0];
    std::string message = params[1];

    std::string prefix = ":" + _users[fd].getNick() + "!"
                       + _users[fd].getUsername() + "@"
                       + _users[fd].getIpAddress();

    if (destination[0] == '#') // é um canal
    {
        std::map<std::string, Channel>::iterator it = _channels.find(destination);
        if (it == _channels.end())
        {
            sendToClient(fd, ":ircserv 403 " + destination + " :No such channel\r\n");
            return ;
        }
        // broadcast para membros do canal
    }
    else // é um utilizador
    {
        std::map<int, User>::iterator it = _users.begin();
        for (; it != _users.end(); it++)
        {
            if (it->second.getNick() == destination)
            {
                sendToClient(it->first, prefix + " PRIVMSG " + destination + " :" + message + "\r\n");
                return ;
            }
        }
        sendToClient(fd, ":ircserv 401 " + destination + " :No such nick\r\n");
    }
}	for (size_t i = 1; i < _polls.size(); i++) {
		if (_users[_polls[i].fd].getNick() != nick)
			continue ;
		return (_users[_polls[i].fd].getFd());
	}
*/
