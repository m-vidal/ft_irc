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
	
		virtual void				runCommand(User &user, std::string command) = 0;
		virtual void				runCommand(User &user, Channel &channel) = 0;

	private:
		std::string					_command;
		std::string					_trailing;
		std::vector<std::string>	_params;
};

#endif
