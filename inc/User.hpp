/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:54:53 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/19 19:32:06 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include <string>
#include <vector>

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
		User(fd, nick, name);

		// getters
		const					std::string &getNick(void) const;
		const					std::string &getUsername(void) const;

		// setters
		void					setNick(const std::string &nick);
		void					setUser(const std::string &user);
};

#endif
