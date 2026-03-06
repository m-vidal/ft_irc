/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:33:23 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/03 19:16:53 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"
#include "../inc/Channel.hpp"
#include <string>
#include <vector>

//constructor destructor
Server::~Server(void) {
	if (_socket != -1)
		close(_socket);
}
Server::Server(unsigned short &port, std::string &password): _onlineUsers(0), _password(password), _socket(socket(AF_INET, SOCK_STREAM, 0)), _port(port) {
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

//commands
void	Server::pass(int fd, std::vector<std::string>& params, std::string trailing) {
	(void)trailing;
	if(_users[fd].isAuthenticated() == true) {
		ircReply(fd, ERR_ALREADYREGISTERED, "PASS", "User already registered!");
		return ;
	}
	if (params.size() == 0) {
		ircReply(fd, ERR_NEEDMOREPARAMS, "PASS", "Not enough parameters");
		return ;
	}
	if (params[0] != _password) {
		ircReply(fd, ERR_PASSWDMISMATCH, "PASS", "Wrong password!");
		return ;
	}
	_users[fd].setPassAccepted();//sets passAccepted to true
}
void Server::nick(int fd, std::vector<std::string>& params, std::string trailing) {
    (void)trailing;
    if (params.empty()) {
        ircReply(fd, ERR_NONICKNAMEGIVEN, "NICK", "No nickname given");
        return;
    }
    const std::string& newNick = params[0];
    if (std::isdigit(newNick[0]) ||
        newNick.find_first_of(" ,*!@:#\n") != std::string::npos) {
        ircReply(fd, ERR_ERRONEUSNICKNAME, "NICK", "Erroneous nickname");
        return;
    }
    User* existing = findUserByNick(newNick);
    if (existing && existing->getFd() != fd) {
        return ircReply(fd, ERR_NICKNAMEINUSE, newNick, "Nickname is already in use");
    }
    std::string oldNick = _users[fd].getNick();
    bool wasAuthenticated = _users[fd].isAuthenticated();
    _users[fd].setNick(newNick);
    if (wasAuthenticated) {
        std::string message = ":" +
            oldNick + "!" +
            _users[fd].getUsername() + "@" +
            _users[fd].getHostname() +
            " NICK :" + newNick;
		ircReply(fd, message);

		std::set<int> notified;
		notified.insert(fd);
		
		for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		    if (it->second.isUser(_users[fd]))
		        broadcastToChannel(it->second, message, notified);
		}
    }
    checkRegistration(fd);
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
	checkRegistration(fd);
}
void	Server::list(int fd, std::vector<std::string> &params, std::string trailing) {
	(void)params;
	(void)trailing;
		if (_users[fd].isAuthenticated() == false) {
		ircReply(fd, ERR_NOTREGISTERED, "LIST", "User not registered");
		return;
	}

	std::map<std::string, Channel>::iterator it;

	for (it = _channels.begin(); it != _channels.end(); ++it) {

		Channel &channel = it->second;

		std::stringstream ss;
		ss << ":ircserv 322 "
		   << getUserNick(fd) << " "
		   << channel.getName() << " "
		   << channel.getUserCount() << " :"
		   << channel.getTopic();

		ircReply(fd, ss.str());
	}

	std::stringstream end;
	end << ":ircserv 323 "
		<< getUserNick(fd)
		<< " :End of /LIST";

	ircReply(fd, end.str());
}
void	Server::names(int fd, std::vector<std::string>& params, std::string trailing) {
	if (_users[fd].isAuthenticated() == false) {
		ircReply(fd, ERR_NOTREGISTERED, "NAMES", "User not registered!");
		return ;
	}		
	if (params.empty()) {
		for (std::map<std::string, Channel>::iterator it = _channels.begin();
			it != _channels.end(); ++it) {
			if (it->second.isUser(_users[fd])) {
				ircReply(fd, ":ircserv NAMES :" + it->second.getName());
				sendUserList(it->second, "NAMES", fd);
			}
		}
		return;
    } else {
		if (_channels.find(params[0]) == _channels.end() || params[0][0] != '#') {
			ircReply(fd, ERR_NOSUCHCHANNEL, "NAMES", "No such channel!");
			return ;
		}
		sendUserList(_channels[params[0]], "NAMES", fd);
	}

}
void	Server::ping(int fd, std::vector<std::string>& params, std::string trailing) {
	(void)trailing;
	if (params.size() == 0) {
		ircReply(fd, ERR_NEEDMOREPARAMS, "PING", "Not enough parameters!");
		return ;
	}
	ircReply(fd, ":ircserv PONG ircserv :ircserv");
}
void	Server::msg(int fd, std::vector<std::string>& params, std::string trailing) {
	if (_users[fd].isAuthenticated() == false) {
		ircReply(fd, ERR_NOTREGISTERED, "PRIVMSG", "User not registered!");
		return ;
	}
	if (params.empty() || params[0].empty()) {
		ircReply(fd, ERR_NORECIPIENT, "PRIVMSG","Empty target.");
		return ;
	}
	if (trailing.empty() == true) {
		ircReply(fd, ERR_NOTEXTTOSEND, "PRIVMSG", "No text to send");
		return ;
	}
	if (params[0][0] == '#') {
		std::map<std::string, Channel>::iterator chanIt = _channels.find(params[0]);
		if (chanIt == _channels.end())
		{
			ircReply(fd, ERR_NOSUCHCHANNEL, "PRIVMSG", "No such channel.");
			return ;
		}
		Channel &channel = chanIt->second;
		if (channel.isUser(_users[fd]) == false) {
			ircReply(fd, ERR_NOTONCHANNEL, "PRIVMSG", "Not on channel.");
			return ;
		}
		User &user = _users[fd];
		std::string message = ":" + user.getNick() + "!" + user.getUsername() + "@" + user.getHostname() + " PRIVMSG " + params[0] + " :" + trailing;
		std::set<int>	notified;
		notified.insert(fd);
		broadcastToChannel(channel, message, notified);
	} else {
		int targetFd = getFdFromNick(params[0]);
		if (targetFd == -1 || _users.find(targetFd) == _users.end())
		{
			ircReply(fd, ERR_NOSUCHNICK, "PRIVMSG", "This user does not exist.");
			return;
		}
		User &target = _users[targetFd];
		std::string message = ":" + _users[fd].getNick() + "!" + _users[fd].getUsername() + "@" + _users[fd].getHostname() + " PRIVMSG " + target.getNick() + " :" + trailing;
		ircReply(target.getFd(), message);
	}
}
void	Server::join(int fd, std::vector<std::string>& params, std::string trailing) {
	(void)trailing;
	// Fix #2: check params is non-empty before any access
	if (params.empty() || params[0].empty()) {
		ircReply(fd, ERR_NEEDMOREPARAMS, "JOIN", "Not enough parameters!");
		return ;
	}
	if (_users[fd].isAuthenticated() == false) {
		ircReply(fd, ERR_NOTREGISTERED, "JOIN", "User not registered!");
		return ;
	}
	if (params[0][0] != '#') {
		ircReply(fd, ERR_NOSUCHCHANNEL, "JOIN", "No such channel!");
		return ;
	}
	// Single find to avoid multiple map lookups
	std::map<std::string, Channel>::iterator chanIt = _channels.find(params[0]);
	if (chanIt != _channels.end() && chanIt->second.isUser(_users[fd])) {
		return;
	}
	if (chanIt == _channels.end()) {
		Channel channel(params[0]);
		_channels.insert(std::make_pair(channel.getName(), channel));

		std::cout << "Channel " << params[0] << " created." << std::endl;

		_channels[params[0]].addUser(_users[fd]);
		_channels[params[0]].addOperator(_users[fd]);
		sendUserList(_channels[params[0]], "JOIN", fd);
	} else {
		_channels[params[0]].addUser(_users[fd]);
		sendUserList(_channels[params[0]], "JOIN", fd);
	}
}
void	Server::notice(int fd, std::vector<std::string>& params, std::string trailing) {
	if (_users[fd].isAuthenticated() == false) {
		return ;
	}
	if (params.empty() || params[0].empty()) {
		return ;
	}
	if (trailing.empty() == true) {
		return ;
	}
	if (params[0][0] == '#') {
		if (_channels.find(params[0]) == _channels.end())
		{
			return ;
		}
		if (_channels[params[0]].isUser(_users[fd]) == false) {
			return ;
		}
		User &user = _users[fd];
		std::string message = ":" + user.getNick() + "!" + user.getUsername() + "@" + user.getHostname() + " NOTICE " + params[0] + " :" + trailing;
		std::set<int>	notified;
		notified.insert(fd);
		broadcastToChannel(_channels[params[0]], message, notified);
	} else {
		int targetFd = getFdFromNick(params[0]);
		if (targetFd == -1 || _users.find(targetFd) == _users.end()) {
			return;
		}
		User &target = _users[targetFd];
		std::string message = ":" + _users[fd].getNick() + "!" + _users[fd].getUsername() + "@" + _users[fd].getHostname() + " NOTICE " + target.getNick() + " :" + trailing;
		ircReply(target.getFd(), message);
	}
}
void	Server::sendUserList(const Channel &channel, const std::string &command, const int &fd) {
    std::stringstream ss;
    std::stringstream ss1;


	User &user = _users[fd];
	std::string message = ":" + user.getNick() +
                      "!" + user.getUsername() +
                      "@" + user.getHostname() +
                      " " + command + " " + channel.getName();
	std::set<int>	notified;
	notified.insert(fd);
	if (command == "JOIN") {
		broadcastToChannel(channel, message, notified);
		ircReply(fd, "JOIN", channel.getName());
	}

    ss << std::setw(3) << std::setfill('0') << RPL_NAMREPLY;
	ircReply(fd, 
		":ircserv " + ss.str() + " " + getUserNick(fd) + " = " + channel.getName() + " :" + channel.getUserNickList()
	);
    ss1 << std::setw(3) << std::setfill('0') << RPL_ENDOFNAMES;
	ircReply(fd, ":ircserv " + ss1.str() + " " + getUserNick(fd) + " " + channel.getName() + " :End of /NAMES list");
}

void	Server::part(int fd, std::vector<std::string> &params, std::string trailing) {
	if (_users[fd].isAuthenticated() == false) {
		ircReply(fd, ERR_NOTREGISTERED, "PART", "User not registered!");
		return ;
	}
	if (params.size() == 0) {
		ircReply(fd, ERR_NEEDMOREPARAMS, "PART", "Not enough params!");
		return ;
	}
	if (_channels.find(params[0]) == _channels.end()) {
		ircReply(fd, ERR_NOSUCHCHANNEL, params[0], "No such channel!");
		return ;
	}
	Channel &channel = _channels[params[0]];
	std::string channelName = channel.getName();

	if (!channel.isUser(_users[fd])) {
		ircReply(fd, ERR_NOTONCHANNEL, channelName, "You're not on that channel.");
		return;
	}
	User &user = _users[fd];
	std::string message = ":" + user.getNick() +
                      "!" + user.getUsername() +
                      "@" + user.getHostname() +
                      " PART " + channelName;

	if (trailing.empty() == false) {
		message += " :" + trailing;
	}
	
	std::set<int>	notified;
	broadcastToChannel(channel, message, notified);

	bool wasOp = channel.isOperator(user);
	channel.removeUser(fd);
	if (wasOp)
		channel.removeOperator(fd);
	if (channel.getUserCount() == 0)
		_channels.erase(channelName);
}

//replies
void	Server::ircReply(int fd, int code, const std::string &command, const std::string &trailing) const{
    std::stringstream ss;

    ss << std::setw(3) << std::setfill('0') << code;
	std::string reply = ":ircserv " + ss.str() + " " + getUserNick(fd) + " " + command  + " :" + trailing + "\r\n";
	send(fd, reply.c_str(), reply.size(), 0);
}
void	Server::ircReply(int fd, const std::string &msg) const{
	std::string reply = msg + "\r\n";
	send(fd, reply.c_str(), reply.size(), 0);
}
void Server::ircReply(int fd, const std::string &command, const std::string &trailing) const {
    std::map<int, User>::const_iterator it = _users.find(fd);
    
    if (it == _users.end()) {
        return; // Or handle error
    }

    const User &user = it->second;

    std::string reply = ":" + user.getNick() +
                        "!" + user.getUsername() +
                        "@" + user.getHostname() +
                        " " + command + " " + trailing + "\r\n";
	std::cout << reply << std::endl;
                        
    send(fd, reply.c_str(), reply.size(), 0);
}

//utilities
void	Server::setknowncommands(void) {
	_commands["PRIVMSG"] = &Server::msg;
	_commands["NOTICE"] = &Server::notice;
	_commands["NAMES"] = &Server::names;
	_commands["LIST"] = &Server::list;
	_commands["PASS"] = &Server::pass;
	_commands["NICK"] = &Server::nick;
	_commands["USER"] = &Server::user;
	_commands["PING"] = &Server::ping;
	_commands["JOIN"] = &Server::join;
	_commands["PART"] = &Server::part;
}
void	Server::checkRegistration(int fd) {
	if (_users[fd].isAuthenticated() == true)
		return ;
	if ((_users[fd].checkIsPassAccepted() == true) && (_users[fd].checkIsUserSet() == true) && (_users[fd].checkIsNickSet() == true)) {
		_users[fd].setIsAuthenticated();
		std::cout << "User: " << _users[fd].getNick() << " is authenticated." << std::endl;
		ircReply(fd, RPL_WELCOME, "", "Welcome to ircserv.");
		ircReply(fd, RPL_YOURHOST, "", "Your host is ircserv, running version 1.0"); //make version and date macros.
		ircReply(fd, RPL_CREATED, "", "This server was created <date>.");
		ircReply(fd, RPL_MYINFO,  "", "ircserv 1.0 <user modes> <chan modes> ");
		incUsers();
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
std::string Server::getUserNick(int fd) const {
	std::map<int, User>::const_iterator it = _users.find(fd);
	if (it == _users.end() || it->second.isAuthenticated() == false || it->second.getNick() == "*") {
		return "*";
	}
	return (it->second.getNick());
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
	std::size_t crlf = str.find("\r\n");
	trimmed = (crlf != std::string::npos) ? str.substr(0, crlf) : str;

	std::size_t commaPos = trimmed.find(" :");
	std::string	trailing;

	if (commaPos != std::string::npos){
		trailing = trimmed.substr(commaPos + 2); // skip " :"
	}
	
	std::string	strBeforeTrailing = trimmed.substr(0, commaPos); //COMMAND param1 param2 ....

	std::string					command;

	std::size_t spacePos = strBeforeTrailing.find(' ');
	
	command = strBeforeTrailing.substr(0, spacePos);
	std::size_t commandLength = command.length();
	for (std::size_t i = 0; i < commandLength; i++) {
		command[i] = std::toupper(command[i]);
	}
	std::string tempArgs;
	if (spacePos != std::string::npos)
		tempArgs = strBeforeTrailing.substr(spacePos + 1);
	std::vector<std::string> args = split(tempArgs);

	std::map<std::string, CommandFunc>::iterator it = _commands.find(command);
	if (it != _commands.end())
		(this->*(it->second))(user.getFd(), args, trailing);
	else if (!command.empty())
		ircReply(user.getFd(), ERR_UNKNOWNCOMMAND, command, "Unknown command");
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
		std::cout << str << std::endl;
		this->parser(_users[fd], str);
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
	close(fd);
	// Remove from all channels before erasing the user
	if (_users.find(fd) != _users.end()) {
		User &user = _users[fd];
		for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ) {
			Channel &channel = it->second;
			if (channel.isUser(user)) {
				bool wasOp = channel.isOperator(user);
				channel.removeUser(fd);
				if (wasOp)
					channel.removeOperator(fd);
			}
			if (channel.getUserCount() == 0)
				it = _channels.erase(it);
			else
				++it;
		}
		if (user.isAuthenticated())
			decUsers();
	}
	_users.erase(fd);
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
				std::cout << "A client with fd: " << clientfd << " has connected" << std::endl;
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
					std::cout << _users[_polls[i].fd].getNick() << ": disconnected " << std::endl;
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

void	Server::incUsers(void) {
	_onlineUsers++;
	std::cout << "Online users: " << _onlineUsers << "." << std::endl;
}
void	Server::decUsers(void) {
	if (_onlineUsers > 0)
		_onlineUsers--;
	else 
		std::cerr << "Number of users already at zero!" << std::endl;
	std::cout << "Online users: " << _onlineUsers << "." << std::endl;
}
// Server.cpp
void Server::broadcastToChannel(const Channel &channel, const std::string &message, std::set<int> &notified) {
	const std::map<int, User> &users = channel.getUsers();
	for (std::map<int, User>::const_iterator it = users.begin(); it != users.end(); ++it) {
		if (notified.find(it->first) == notified.end()) {
			ircReply(it->first, message);
			notified.insert(it->first);
		}
	}
}
