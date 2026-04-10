/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 14:33:23 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/19 14:04:26 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"

// constructor destructor

Command::Command() : handler(NULL), minArgs(0) {}
Command::Command(CommandFunc h, int args) : handler(h), minArgs(args) {}

Server::~Server(void)
{
    if (_socket)
        close(_socket);
}

Server::Server(unsigned short &port, std::string &password, std::string name)
    : _onlineUsers(0), _password(password), _socket(socket(AF_INET, SOCK_STREAM, 0)),
      _port(port), _serverName(name), _creationTime(std::time(NULL)), mode_chars("itkln")
{
    if (!checkPassword(password))
        throw std::runtime_error("Error: password too weak!");

    if (_socket < 0)
        throw std::runtime_error("Error: failure in the socket server creation!");

    // 1. Set options IMMEDIATELY after creating the socket
    int opt = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw std::runtime_error("Error: failure in setsockopt!");

    // 2. Clear the struct and set address
    memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = INADDR_ANY;

    // 3. Now bind (The OS now knows REUSEADDR is allowed)
    if (bind(_socket, reinterpret_cast<struct sockaddr *>(&_addr), sizeof(_addr)) < 0)
    {
        std::cerr << "Bind error: " << strerror(errno) << " " << port << std::endl;
        throw std::runtime_error("Error: failure to associate the port.");
    }

    setknowncommands();
    initReplies();

    is_running = false;
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
    std::cout << "Client [" << fd << "]  disconnected" << std::endl;
    decUsers();
}

void Server::listenMode()
{
    initPoll();
    printBanner();

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
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    // Pass the address structure to accept
    int fd = accept(_socket, (struct sockaddr *)&clientAddr, &clientLen);
    if (fd == -1)
        return;

    char hostname[NI_MAXHOST];
    // getnameinfo converts the raw address into a human-readable hostname or IP
    int res = getnameinfo((struct sockaddr *)&clientAddr, clientLen,
                          hostname, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

    std::string finalHost;
    if (res == 0)
        finalHost = hostname;
    else
    {
        char *ip_ptr = inet_ntoa(clientAddr.sin_addr);
        finalHost = (ip_ptr) ? ip_ptr : "unknown";
    }

    fcntl(fd, F_SETFL, O_NONBLOCK);
    _polls.push_back((struct pollfd){fd, POLLIN, 0});

    // Pass the hostname to your User constructor
    _users.insert(std::make_pair(fd, User(fd, finalHost)));

    std::cout << "New connection: FD " << fd << " from " << finalHost << std::endl;
}

void Server::handleClientData(size_t &idx)
{
    char buffer[512]; // IRC messages are capped at 512 bytes
    int fd = _polls[idx].fd;
    ssize_t n = recv(fd, buffer, sizeof(buffer) - 1, 0);

    if (n <= 0)
    {
        std::vector<std::string> msg;
        msg.push_back("Remote host closed the connection");
        quit(fd, msg);
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

    std::cout << "\nbuffer [" << fd << "] " << buf << std::endl;
    while ((pos = buf.find("\r\n")) != std::string::npos)
    {
        std::string line = buf.substr(0, pos);
        buf.erase(0, pos + 2);
        _users[fd].clearBuffer(pos + 2);

        if (!line.empty())
        {
            this->parseLine(fd, line);
            const char *raw = "- - - - - - - - - - - - - - - - - - \n";
            write(1, raw, 37);
        }
    }
}

void Server::parseLine(int fd, std::string line)
{
    std::string command;
    std::vector<std::string> args;
    std::string trailing;

    // 1. Check for the colon (the "multi-word" parameter)
    size_t colonPos = line.find(" :");
    if (colonPos != std::string::npos)
    {
        trailing = line.substr(colonPos + 1); // Get the : and everything after"
        line = line.substr(0, colonPos);      // Keep only the part before " :"
    }

    // 2. Parse the command and middle parameters
    std::stringstream ss(line);
    ss >> command;
    for (size_t i = 0; i < command.length(); ++i)
        command[i] = std::toupper(command[i]);

    std::string temp;
    while (ss >> temp)
        args.push_back(temp);

    // 3. If we found a colon part, add it as the final argument
    if (colonPos != std::string::npos)
    {
        if (trailing != ":")
            args.push_back(trailing);
    }
    // 4. Special case: If there was no colon, but there is no "trailing",
    // it doesn't matter. The last word in args is already the "trailing".
    // Example: KICK #chan Bob -> args[0] is #chan, args[1] is Bob.

    // Debug output
    std::cout << "cmd = " << command << "\n";
    for (size_t i = 0; i < args.size(); i++)
        std::cout << "param[" << i << "] = " << args[i] << "\n";
    std::cout << "trailing = " << trailing << "\n";

    this->executeCommand(fd, command, args);
}

void Server::executeCommand(int fd, std::string &cmd, std::vector<std::string> &args)
{
    std::map<int, User>::iterator it_user = _users.find(fd);
    if (it_user == _users.end())
        throw std::runtime_error("User not found in map (T - T)");
    User &user = it_user->second;
    std::map<std::string, Command>::iterator it = _commands.find(cmd);
    if (it == _commands.end() && user.isAuthenticated())
        return sendNumeric(fd, ERR_UNKNOWNCOMMAND, cmd);
    else if (it == _commands.end() && !user.isAuthenticated())
        return;

    std::size_t params_needed = it->second.minArgs;

    if (args.size() < params_needed)
        return sendNumeric(fd, ERR_NEEDMOREPARAMS, cmd);

    if (!user.checkIsPassAccepted() && cmd != "PASS")
        return sendNumeric(fd, ERR_NOTREGISTERED, "*");

    if (!user.isAuthenticated() && cmd != "PASS" && cmd != "NICK" && cmd != "USER")
        return sendNumeric(fd, ERR_NOTREGISTERED, "*");

    std::cout << "params given " << args.size() << "\n";
    std::cout << "params needed " << params_needed << "\n";

    (this->*(it->second.handler))(fd, args); // call command handler
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

// 1. For Multi-Channel events (like NICK or QUIT)
void Server::sendToChannel(const Channel &chan, const std::string &msg, std::set<int> &notified)
{
    const std::map<int, Member> &members = chan.getMembers();

    for (std::map<int, Member>::const_iterator it = members.begin(); it != members.end(); ++it)
    {
        int fd = it->first;
        if (notified.find(fd) == notified.end())
        {
            this->sendToClient(fd, msg);
            notified.insert(fd);
        }
    }
}

// 2. For Single-Channel events (like PRIVMSG or KICK)
void Server::sendToChannel(const Channel &chan, const std::string &msg, int userFd)
{
    const std::map<int, Member> &members = chan.getMembers();

    for (std::map<int, Member>::const_iterator it = members.begin(); it != members.end(); ++it)
    {
        int fd = it->first;
        if (fd != userFd)
        {
            this->sendToClient(fd, msg);
        }
    }
}

void Server::sendToUserChannels(const User &user, const std::string &msg)
{
    std::set<int> notified;
    // We seed the set with the sender so they don't get their own message back
    notified.insert(user.getFd());

    std::map<std::string, Channel>::iterator it;
    for (it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (it->second.isMember(user.getFd()))
        {
            this->sendToChannel(it->second, msg, notified);
        }
    }
}

void Server::sendToClient(int fd, std::string rawMsg)
{
    rawMsg += "\r\n";
    // In production/42 eval, it's good to check if send() fails
    if (send(fd, rawMsg.c_str(), rawMsg.size(), 0) == -1)
    {
        // Log error or handle disconnected peer if necessary
    }
}

void Server::checkRegistration(int fd)
{
    std::map<int, User>::iterator it = _users.find(fd);

    if (it == _users.end())
        throw std::runtime_error("Non auth user not _users");
    User &user = it->second;
    if ((user.checkIsPassAccepted() == true) && (user.checkIsUserSet() == true) && (user.checkIsNickSet() == true))
    {
        user.setIsAuthenticated();
        std::cout << "User: " << user.getNick() << " is authenticated." << std::endl;
        sendNumeric(fd, RPL_WELCOME, "Welcome to " + _serverName);
        sendNumeric(fd, RPL_YOURHOST, "Your host is " + _serverName + ", running version 1.0");
        sendNumeric(fd, RPL_CREATED, "This " + _serverName + " was created " + timeToStr(_creationTime));
        sendNumeric(fd, RPL_MYINFO, _serverName + " 1.0 ~NA +" + mode_chars);
        incUsers();

        _users.insert(std::make_pair(fd, User(user)));
        std::cout << "User has been registered!\n";
    }
}