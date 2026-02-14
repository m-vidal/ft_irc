/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 18:13:32 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/14 14:45:10 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "User.hpp"
# include <sys/socket.h>
# include <list>
# include <map>
# include <string>
# include <sys/poll.h>
# include <vector>

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

