/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 18:07:14 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/03 18:18:10 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <string>
class Command {

	public:
		Command();
		~Command();
		Command(const Command &other);
		Command &operator=(const Command &other);

	private:
		std::string	_prefix;
		std::string _command;
		std::string *_params;
		std::string	_trailing;

};

#endif
