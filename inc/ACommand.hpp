/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 18:07:14 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/05 17:33:36 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <string>
class ACommand {

	public:
		ACommand();
		~ACommand();
		ACommand(const ACommand &other);
		ACommand &operator=(const ACommand &other);

	private:
		std::string	_prefix;
		std::string _command;
		std::string *_params;
		std::string	_trailing;

};

#endif
