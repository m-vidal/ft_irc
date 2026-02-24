/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 18:07:14 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/21 18:18:44 by atambo           ###   ########.fr       */
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
	
		virtual void				runCommand() = 0;
		virtual void				runCommand() = 0;

	private:
		std::string					_command;
		std::string					_trailing;
		std::vector<std::string>	_params;
		User&						_user;
		Channel&					_channel;
};



#endif
