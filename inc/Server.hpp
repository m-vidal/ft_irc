/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 18:13:32 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/28 21:31:24 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "replyCodes.hpp"
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/poll.h>
# include <stdexcept>
# include <iostream>
# include <unistd.h>
# include "User.hpp"
# include <cstdlib>
# include <iomanip>
# include <sstream>
# include <fcntl.h>
# include <string>
# include <cctype>
# include <vector>
# include <list>
# include <map>

class Server {

	public:
		bool is_running;

		Server(unsigned short &port, std::string &password);
		~Server();

		void								listenMode();

	private:
		void								processMessage(int fd, std::string str);
		void								sendToClient(int fd, std::string str);
		void								parser(User &user, std::string &str);
		bool								checkPassword(std::string password);
		void								disconnectClient(int fd);
		void								setknowncommands();

		typedef								void (Server::*CommandFunc)(int fd, std::vector<std::string>& params, std::string trailing);

		void								pass(int fd, std::vector<std::string>& params, std::string trailing);
		void								nick(int fd, std::vector<std::string>& params, std::string trailing);
		void								user(int fd, std::vector<std::string>& params, std::string trailing);
		void								ping(int fd, std::vector<std::string>& params, std::string trailing);
		void								msg(int fd, std::vector<std::string>& params, std::string trailing);


		int									getFdFromNick(std::string nick);
		User								*findUserByNick(const std::string& nick);

		void								ircReply(int fd, int code, const std::string &command, const std::string &trailing);
		void								ircReply(int fd, const std::string &command, const std::string &trailing);
		void								ircReply(int fd, const std::string &msg);
		std::string							getUserNick(int fd) const;
		void								checkRegistration(int fd);
		void								incUsers(void);
		void								decUsers(void);
		
		size_t								_onlineUsers;
		std::map<std::string, CommandFunc>	_commands;
		std::list<Channel>					_channels;
		const std::string					_password;
		const short							_socket;
		std::map<int, User>					_users; // um mapa porque assim conseguimos identificar cada user pelo seu fd
		std::vector<pollfd>					_polls;		//para controlar os eventos de cada cliente
		struct sockaddr_in 					_addr;
		const short							_port;
};

#endif
