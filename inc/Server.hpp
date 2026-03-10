/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 18:13:32 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/10 12:55:09 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/poll.h>
# include <iostream>
# include <unistd.h>
# include "User.hpp"
# include <sstream>
# include <fcntl.h>
# include <string>
# include <vector>
# include <list>
# include <map>

class Server {

	public:
		Server(unsigned short &port, std::string &password);
		~Server();

		void	listenMode();
		void	turnOff();

	private:
		void	processMessage(int fd, std::string str);
		void	sendToClient(int fd, std::string str);
		void	parser(User &user, std::string &str);
		void	disconnectClient(int fd);
		void	setknowscommands();

		typedef void (Server::*CommandFunc)(int fd, std::vector<std::string>& params, std::string trailing);

		void	quit(int fd, std::vector<std::string>& Params, std::string trailing);
		void	msg(int fd, std::vector<std::string>& params, std::string trailing);
		void	autUser(int fd, std::string str);

		int		getFdFromNick(std::string nick);

		bool								_received_sig;
		bool								is_running;
		std::map<std::string, CommandFunc>	_commands;
		std::list<Channel>					_channels;
		const std::string					_password;
		const int							_socket;
		std::map<int, User>					_users; // um mapa porque assim conseguimos identificar cada user pelo seu fd
		std::vector<pollfd>					_polls;		//para controlar os eventos de cada cliente
		struct sockaddr_in 					_addr;
		const short							_port;
};

#endif
