/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:32:05 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/15 01:16:25 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>


class User { };

typedef struct Member {
	bool isoperator;
	User &user;
}

class Channel {

	private:
		std::string			name;
		std::list<Member>	membs;

	public:
		Channel(const std::string &name);
		~Channel();

		const std::string &getName(void) const;
		void setName(const std::string &user);
		void makeoperator(const std::vector<Users>::iterator pos);
};

#endif
