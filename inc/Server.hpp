/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 18:13:32 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/24 15:13:45 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "User.hpp"
# include <list>
# include <string>

class Server {
	private:
		const short				_port;
		std::list<User>			_users;
		const short				_socket;
		std::list<Channel>		_channels;
		const std::string		_password;
		
	public:
		User	*getUser(std::string &user_name);
		Channel	*getChannel(std::string &channel_name);
};

#endif

