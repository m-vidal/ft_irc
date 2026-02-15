/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 18:13:32 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/14 20:48:00 by mvidal           ###   ########.fr       */
=======
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 18:13:32 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/14 14:45:10 by marcsilv         ###   ########.fr       */
>>>>>>> upstream/temp
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

<<<<<<< HEAD
# include <sys/socket.h>
# include <sys/poll.h>
# include "User.hpp"
# include <sstream>
# include <string>
# include <vector>
# include <list>
# include <map>
=======
# include "User.hpp"
# include <sys/socket.h>
# include <list>
# include <map>
# include <string>
# include <sys/poll.h>
# include <vector>
>>>>>>> upstream/temp

class Server {

	public:
<<<<<<< HEAD
		static bool is_running;

=======
>>>>>>> upstream/temp
		Server(unsigned short &port, std::string &password);
		~Server();

	private:
<<<<<<< HEAD
		std::list<Channel>		_channels;
		const std::string		_password;
		const short				_socket;
		std::map<int, User>		_users; // um mapa porque assim conseguimos identificar cada user pelo seu fd
		std::vector<pollfd>		_polls;		//para controlar os eventos de cada cliente
		const short				_port;
};

#endif
=======
		int						_fd;    //acredito que o server também deve ter um fd, se não se encontrar necessário pode apagar
		const short				_port;
		std::map<int, User>		_users; // um mapa porque assim conseguimos identificar cada user pelo seu fd
		const short				_socket;
		std::list<Channel>		_channels;
		const std::string		_password;
		std::vector<pollfd>		_polls;		//para controlar os eventos de cada cliente
};

#endif

>>>>>>> upstream/temp
