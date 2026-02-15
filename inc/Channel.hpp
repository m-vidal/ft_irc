/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:32:05 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/15 01:40:16 by atambo           ###   ########.fr       */
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
		std::string			topic;
		size_t				maxmembs;
		short				modes;
		std::list<Member>	membs;

	public:
		Channel(const std::string &name);
		~Channel();
		// --------------------
		// getters and printers
		const std::string &getName(void) const;
		const std::string &getTopic(void) const;
		const std::string &MaxMembers(void) const;
		const std::string &getModes(void) const;
		const std::string printMembers(void) const;
		// ---------------------
		// command functions
		void setName(const User &exec, const std::string name);
		void setTopic(const User &exec, const std::string &topic);
		void setMaxMebs(const User &exec, const size_t len);
		void setModes(const User &exec, const short modes);
		void makeoperator(const User &exec, const &Member);
};

#endif
