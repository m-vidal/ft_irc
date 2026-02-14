/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 18:13:32 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/14 19:42:15 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/socket.h>
# include <sys/poll.h>
# include "User.hpp"
# include <sstream>
# include <string>
# include <vector>
# include <list>
# include <map>

class Server {

	public:
		Server(unsigned short &port, std::string &password);
		~Server();

	private:
		int						_fd;    //acredito que o server também deve ter um fd, se não se encontrar necessário pode apagar
		const short				_port;
		std::map<int, User>		_users; // um mapa porque assim conseguimos identificar cada user pelo seu fd
		const short				_socket;
		std::list<Channel>		_channels;
		const std::string		_password;
		std::vector<pollfd>		_polls;		//para controlar os eventos de cada cliente
};

#endif

