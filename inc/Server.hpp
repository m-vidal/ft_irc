/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 18:13:32 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/12 15:25:08 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <netdb.h>
#include <fcntl.h>
#include <string>
#include <cctype>
#include <vector>
#include <list>
#include <set>
#include <map>

#include "replyCodes.hpp"

class User;
class Channel;
class Server;

typedef void (Server::*CommandFunc)(int fd, std::vector<std::string> &params, std::string trailing);
struct Command
{
	CommandFunc handler;
	int minArgs;
	bool needTrail;

	Command();
	Command(CommandFunc h, int args, bool trail);
};

class Server
{

public:
	bool is_running;
	static const int MAX_CHAN_KEY_LEN = 23;
	static const size_t MAX_CHAN_NAME = 200;
	static const size_t MAX_JOINED_CHAN = 10;

	Server(unsigned short &port, std::string &password, std::string name);
	~Server();

	void listenMode();
	std::string formatNumeric(int code, const std::string &nick, const std::string &params, const std::string &trailing);
	std::string formatMessage(const User &source, const std::string &command, const std::string &params, const std::string &trailing);
	void sendToClient(int fd, const std::string &rawMsg);
	void sendUserList(const Channel &channel, int fd);
	void sendNumeric(int fd, int code, const std::string &params, const std::string &trailing);
	void sendNumeric(Channel &channel, int fd, int code, const std::string &params, const std::string &trailing);

private:
	// data ---------------------------
	size_t _onlineUsers;
	std::map<std::string, Command> _commands;
	std::map<std::string, Channel> _channels;
	const std::string _password;
	const short _socket;
	std::map<int, User> _users;
	std::vector<pollfd> _polls;
	struct sockaddr_in _addr;
	const short _port;
	const std::string _serverName;

	// methods ------------------------
	void initPoll();
	void acceptNewClient();

	void handleClientData(size_t &idx);
	void consumeBuffer(int fd);

	void processMessage(int fd, std::string str);
	void parseLine(int fd, std::string line);
	void executeCommand(int fd, std::string &cmd, std::vector<std::string> &args, std::string &trailing);

	bool checkPassword(std::string password);
	void disconnectClient(int fd);
	void setknowncommands();

	// command functions ------------------------------------------------------
	void msg(int fd, std::vector<std::string> &params, std::string trailing);
	void pass(int fd, std::vector<std::string> &params, std::string trailing);
	void nick(int fd, std::vector<std::string> &params, std::string trailing);
	void user(int fd, std::vector<std::string> &params, std::string trailing);
	void ping(int fd, std::vector<std::string> &params, std::string trailing);
	void join(int fd, std::vector<std::string> &params, std::string trailing);
	void part(int fd, std::vector<std::string> &params, std::string trailing);
	void invite(int fd, std::vector<std::string> &params, std::string trailing);
	void topic(int fd, std::vector<std::string> &params, std::string trailing);

	// mode command -----------------------------------------------------------
	void mode(int fd, std::vector<std::string> &params, std::string trailing);
	void applyModeString(int fd, std::vector<std::string> &params, std::string trailing, Channel &channel);
	bool mode_k(int fd, std::vector<std::string> &params, Channel &channel, size_t j);
	// ------------------------------------------------------------------------

	int getFdFromNick(std::string nick);
	User *findUserByNick(const std::string &nick);

	void sendToChannel(const Channel &chan, const std::string &msg, std::set<int> &notified);
	void sendToChannel(const Channel &chan, const std::string &msg, int skipFd);
	void sendToUserChannels(const User &user, const std::string &msg);

	std::string getUserNick(int fd) const;
	void checkRegistration(int fd);
	void incUsers(void);
	void decUsers(void);
	void printBanner();
};

bool valid_channel_name(const std::string &name);
std::string timeToStr(time_t time);
std::string ircToLower(std::string str);

#endif
