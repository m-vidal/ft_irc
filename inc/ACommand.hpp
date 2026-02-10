/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 18:07:14 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/09 19:14:07 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "User.hpp"

#include <string>

class ACommand {
		
	public:
		ACommand();
		~ACommand();
	
		void		runCommand(User user);
		void		runCommand(User user, Channel channel);

	private:
		std::string _command;
		std::string *_params;
		std::string	_trailing;
};

#endif
