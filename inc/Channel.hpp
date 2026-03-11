/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:32:05 by marcsilv          #+#    #+#             */
/*   Updated: 2026/03/03 19:22:13 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <set>
# include <map>
# include <string>
# include "User.hpp"

class Channel {

	public:
		Channel() { };
		~Channel() { };
		Channel(const std::string &name);
			
		std::string			getName(void) const;
		std::string			getTopic(void) const;
		// getUserNickList now needs live user map to build accurate nick list
		std::string			getUserNickList(const std::map<int, User> &allUsers) const;
		size_t				getUserCount(void) const;
		const std::set<int>	&getUserFds(void) const;

		void				setName(const std::string &newName);

		void				addUser(int fd);
		void				addOperator(int fd);

		void				removeUser(int fd);
		void				removeOperator(int fd);

		bool				isUser(int fd) const;
		bool				isOperator(int fd) const;
		// Convenience overloads that accept a User object
		bool				isUser(const User &user) const;
		bool				isOperator(const User &user) const;

	private:
		std::string			_name;
		std::set<int>		_userFds;
		std::string			_topic;
		std::set<int>		_operatorFds;
		//modes
};

#endif
