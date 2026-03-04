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

#include <map>
# include <string>
# include "User.hpp"

class Channel {

	public:
		Channel() { };
		~Channel() { };
		Channel(const std::string &name);
			
		std::string			getName(void) const;
		std::string			getOperators(void) const;
		std::string			getUserNickList(void) const;

		void				setName(const std::string &newName);

		void				addUser(const User &user);
		void				addOperator(const User &user);

		void				removeUser(const int &fd);
		void				removeOperator(const int &fd);

		bool				isUser(const User &user) const ;
		bool				isOperator(const User &user) const ;

		void				incUsers(void);
		void				decUsers(void);
		void				incOperators(void);
		void				decOperators(void);
	private:
		std::string			_name;
		std::map<int, User>	_users;
		std::string			_topic;
		size_t				_nbUsers;
		std::map<int, User>	_operators;
		size_t				_nbOperators;
		//modes
};
