/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 18:13:32 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/15 02:14:02 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/socket.h>
# include <sys/poll.h>
# include <unistd.h>
# include "User.hpp"
# include <sstream>
# include <string>
# include <vector>
# include <list>
# include <map>

class Server {

	public:
		static bool is_running;

		Server(unsigned short &port, std::string &password);
		~Server();

	private:
		std::list<Channel>		_channels;
		const std::string		_password;
		const short				_socket;
		std::map<int, User>		_users; // um mapa porque assim conseguimos identificar cada user pelo seu fd
		std::vector<pollfd>		_polls;		//para controlar os eventos de cada cliente
		const short				_port;
};

#endif
