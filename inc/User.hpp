/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:54:53 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/09 18:58:58 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include "Primary.hpp"

class User {
	private:
		std::string				_nickname;
		std::string 			_username;
		bool					_isOperator;
		std::vector<Channel>	channels;

	public:
		User(void);
		~User(void);
		User(const User &other);
		User &operator=(const User &other);

		// getters
		const std::string &getNick(void) const;
		const std::string &getUser(void) const;
		bool isOperator(void) const;

		// setters
		void setNick(const std::string &nick);
		void setUser(const std::string &user);
};

#endif
