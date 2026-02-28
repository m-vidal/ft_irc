/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 17:54:53 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/28 16:23:19 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include <string>
#include <vector>

class Channel { };

class User {
	private:
		bool					_isAuthenticated;
		size_t					_interactions;
		bool					_isOperator;
		std::string				_ipAddress;
		//std::vector<Channel>	_channels;	//acho que se vai apagar depois
		std::string				_nickname;
		std::string 			_username;
		bool					_stepOne;
		std::string				_buffer;
		const short				_fd;

	public:
		User(int fd);
		User(void);
		~User(void);

		// getters
		const					std::string &getUsername(void) const;
		const					std::string &getNick(void) const;
		void					clearBuffer( size_t pos );
		bool					isOperator(void) const;
		std::string				getBuffer( void );
		size_t					getInteractions();
		bool					isAuthenticated();
		bool					getStepOne();
		int						getFd(void) ;

		// setters
		void					setNick(const std::string &nick);
		void					setUser(const std::string &user);
		void					appendToBuffer(std::string data);
		
		void					incInteractions();
		void					authenticate();
		void					setStepOne();
};

#endif
