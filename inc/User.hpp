/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:54:53 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/10 18:16:03 by marcsilv         ###   ########.fr       */
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
		std::vector<Channel>	_channels;	//acho que se vai apagar depois
		bool					_isOperator;
		std::string				_ipAddress;

	public:
		User(void);
		~User(void);
		User(const User &other);
		User &operator=(const User &other);

		// getters
		const					std::string &getNick(void) const;
		const					std::string &getUsername(void) const;
		bool					isOperator(void) const;

		// setters
		void					setNick(const std::string &nick);
		void					setUser(const std::string &user);
};

#endif
