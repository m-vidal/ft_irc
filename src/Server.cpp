/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:33:23 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/06 15:25:07 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

// constructor destructor
Server::~Server(void)
{
	if (_socket)
		close(_socket);
}
Server::Server(unsigned short &port, std::string &password) : _onlineUsers(0), _password(password), _socket(socket(AF_INET, SOCK_STREAM, 0)), _port(port)
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
	setknowncommands();
	is_running = false;
}

void Server::sendUserList(const Channel &channel, const int &fd)
{
	std::stringstream ss;
	std::stringstream ss1;

	User &user = _users[fd];
	std::string message = ":" + user.getNick() +
						  "!" + user.getUsername() +
						  "@" + user.getHostname() +
						  " JOIN " + channel.getName();
	std::set<int> notified;
	notified.insert(fd);
	broadcastToChannel(channel, message, notified);
	ircReply(fd, "JOIN", channel.getName());

	ss << std::setw(3) << std::setfill('0') << RPL_NAMREPLY;
	ircReply(fd,
			 ":ircserv " + ss.str() + " " + getUserNick(fd) + " = " + channel.getName() + " :" + channel.getUserNickList());
	ss1 << std::setw(3) << std::setfill('0') << RPL_ENDOFNAMES;
	ircReply(fd, ":ircserv " + ss1.str() + " " + getUserNick(fd) + " " + channel.getName() + " :End of /NAMES list");
}

// utilities
void Server::setknowncommands(void)
{
	// _commands["PRIVMSG"] = &Server::msg;
	_commands["/PASS"] = &Server::pass;
	_commands["/NICK"] = &Server::nick;
	_commands["/USER"] = &Server::user;
	_commands["/PING"] = &Server::ping;
	_commands["/JOIN"] = &Server::join;
	_commands["/PART"] = &Server::part;
}

void Server::checkRegistration(int fd)
{
	if (_users[fd].isAuthenticated() == true)
		return;
	if ((_users[fd].checkIsPassAccepted() == true) && (_users[fd].checkIsUserSet() == true) && (_users[fd].checkIsNickSet() == true))
	{
		_users[fd].setIsAuthenticated();
		std::cout << "User: " << _users[fd].getNick() << " is authenticated." << std::endl;
		ircReply(fd, RPL_WELCOME, "", "Welcome to ircserv.");
		ircReply(fd, RPL_YOURHOST, "", "Your host is ircserv, running version 1.0"); // make version and date macros.
		ircReply(fd, RPL_CREATED, "", "This server was created <date>.");
		ircReply(fd, RPL_MYINFO, "", "ircserv 1.0 <user modes> <chan modes> ");
		incUsers();
	}
}

User *Server::findUserByNick(const std::string &nick)
{
	for (size_t i = 1; i < _polls.size(); ++i)
	{
		int fd = _polls[i].fd;

		// opcional: ignorar o fd do próprio servidor se estiver no índice 0
		if (_users.find(fd) == _users.end())
			continue;

		if (_users[fd].getNick() == nick)
			return (&_users[fd]);
	}
	return (NULL);
}
std::string Server::getUserNick(int fd) const
{
	std::map<int, User>::const_iterator it = _users.find(fd);
	if (it == _users.end() || it->second.isAuthenticated() == false || it->second.getNick() == "*")
	{
		return "*";
	}
	return (it->second.getNick());
}
int Server::getFdFromNick(std::string nick)
{
	for (size_t i = 1; i < _polls.size(); i++)
	{
		if (_users[_polls[i].fd].getNick() != nick)
			continue;
		return (_users[_polls[i].fd].getFd());
	}
	return (-1);
}
std::vector<std::string> split(const std::string &input)
{
	std::vector<std::string> result;
	std::string token;
	std::string::size_type i = 0;
	const std::string::size_type n = input.size();

	while (i < n && (input[i] == ' ' || input[i] == '\t'))
		++i;

	while (i < n)
	{
		token.clear();
		while (i < n && input[i] != ' ' && input[i] != '\t')
		{
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

bool Server::checkPassword(std::string password)
{
	// bool hasUpper = false;
	// bool hasLower = false;
	// bool hasNumbr = false;
	// bool hasSmbl = false;

	// if (password.size() < 8)
	// 	return (false);
	// for (size_t i = 0; i < password.size(); i++)
	// {
	// 	if (std::isupper(password[i]))
	// 		hasUpper = true;

	// 	else if (std::islower(password[i]))
	// 		hasLower = true;
	// 	else if (std::isdigit(password[i]))
	// 		hasNumbr = true;
	// 	else
	// 		hasSmbl = true;
	// }
	// if (hasUpper && hasLower && hasNumbr && hasSmbl)
	// 	return (true);
	// return (false);
	(void)password;
	return true;
}

void Server::sendToClient(int fd, std::string str)
{
	send(fd, str.c_str(), str.size(), 0);
}
void Server::disconnectClient(int fd)
{
	for (size_t i = 0; i < _polls.size(); i++)
	{
		if (_polls[i].fd == fd)
		{
			_polls.erase(_polls.begin() + i);
			break;
		}
	}
	close(fd);
	_users.erase(fd);
	decUsers();
}

void Server::listenMode()
{
	initPoll();
	std::cout << "Server listening on port " << _port << std::endl;

	while (is_running)
	{
		if (poll(_polls.data(), _polls.size(), -1) < 0)
			break;

		for (size_t i = 0; i < _polls.size(); ++i)
		{
			if (!(_polls[i].revents & POLLIN))
				continue;

			if (_polls[i].fd == _socket)
				acceptNewClient();
			else
				handleClientData(i);
		}
	}
}

void Server::initPoll()
{
	listen(_socket, 10);
	fcntl(_socket, F_SETFL, O_NONBLOCK);
	_polls.push_back((struct pollfd){_socket, POLLIN, 0});
	is_running = true;
}

void Server::acceptNewClient()
{
	int fd = accept(_socket, NULL, NULL);
	if (fd == -1)
		return;

	fcntl(fd, F_SETFL, O_NONBLOCK);
	_polls.push_back((struct pollfd){fd, POLLIN, 0});
	_users.insert(std::make_pair(fd, User(fd)));

	std::cout << "New connection: FD " << fd << std::endl;
}

void Server::handleClientData(size_t &idx)
{
	char buffer[512]; // IRC messages are capped at 512 bytes
	int fd = _polls[idx].fd;
	ssize_t n = recv(fd, buffer, sizeof(buffer) - 1, 0);

	if (n <= 0)
	{
		disconnectClient(fd);
		return;
	}
	buffer[n] = '\0';
	_users[fd].appendToBuffer(buffer);
	consumeBuffer(fd);
}

void Server::consumeBuffer(int fd)
{
	std::string buf = _users[fd].getBuffer();
	size_t pos;

	while ((pos = buf.find("\r\n")) != std::string::npos)
	{
		std::cout << "buffer[" << fd << "]" << buf << "\n";
		std::string line = buf.substr(0, pos);
		buf.erase(0, pos + 2);

		if (!line.empty())
			this->parseLine(fd, line);
		std::cout << "buffer[" << fd << "]" << buf << "\n";
	}
}

void Server::parseLine(int fd, std::string line)
{
	std::string command, trailing;
	std::vector<std::string> args;

	// Split trailing first to protect spaces inside messages
	size_t colonPos = line.find(" :");
	if (colonPos != std::string::npos)
	{
		trailing = line.substr(colonPos + 2);
		line = line.substr(0, colonPos);
	}

	std::stringstream ss(line);
	ss >> command;
	// for (size_t i = 0; i < command.length(); ++i)
	// 	command[i] = std::toupper(command[i]);

	std::string temp;
	while (ss >> temp)
		args.push_back(temp);

	this->executeCommand(fd, command, args, trailing);
}

void Server::executeCommand(int fd, std::string &cmd, std::vector<std::string> &args, std::string &trailing)
{
	User &user = _users[fd];

	// 1. Check if command exists
	std::map<std::string, CommandFunc>::iterator it = _commands.find(cmd);
	if (it == _commands.end())
	{
		ircReply(fd, ERR_UNKNOWNCOMMAND, cmd, "Unknown command");
		return;
	}
	if (!user.checkIsPassAccepted() && cmd != "/PASS")
	{
		ircReply(fd, ERR_PASSWDMISMATCH, "*", "passowrd");
		return;
	}
	if (!user.isAuthenticated() && cmd != "/PASS" && cmd != "/NICK" && cmd != "/USER")
	{
		ircReply(fd, ERR_NOTREGISTERED, "*", "You have not registered");
		return;
	}
	// 3. Final Execution
	(this->*(it->second))(fd, args, trailing);
}

void Server::incUsers(void)
{
	_onlineUsers++;
	std::cout << "Online users: " << _onlineUsers << "." << std::endl;
}
void Server::decUsers(void)
{
	if (_onlineUsers > 0)
		_onlineUsers--;
	else
		std::cerr << "Number of users already at zero!" << std::endl;
	std::cout << "Online users: " << _onlineUsers << "." << std::endl;
}
// Server.cpp
void Server::broadcastToChannel(const Channel &channel, const std::string &message, std::set<int> &notified)
{
	const std::map<int, User> &users = channel.getUsers();
	for (std::map<int, User>::const_iterator it = users.begin(); it != users.end(); ++it)
	{
		if (notified.find(it->first) == notified.end())
		{
			ircReply(it->first, message);
			notified.insert(it->first);
		}
	}
}
