/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:32:05 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/09 18:58:31 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Primary.hpp"

class Channel {

	public:
		Channel();
		~Channel();
		Channel(const Channel &other);
		Channel &operator=(const Channel &other);

		const std::string &getName(void) const;

		void setName(const std::string &user);

	private:
		std::string	name;
		std::vector<User> users;

};

#endif
