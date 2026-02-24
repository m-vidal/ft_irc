/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:54:53 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/24 15:16:44 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include <string>
#include <vector>
//--------------------------------------------------
#include "inc/Channel.hpp"

class Channel { };

class User {
	private:
		const short				_fd;
		std::string				_nickname;
		std::string 			_username;
		std::vector<Channel>	_channels;
		std::string				_inbuf;
		std::string				_outbuf;

	public:
		User(short fd, const std::string &nick, const std::string &name);

		// getters
		const std::string getNick(void) const;
		const std::string getName(void) const;
		// setters
		void	setNick(const std::string &nick);
		void	setName(const std::string &user);
		// ------------------------------------------
		void	write();
		void	read();
};

#endif
