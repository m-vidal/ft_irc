/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:32:05 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/19 10:22:25 by atambo           ###   ########.fr       */
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
		unsigned int		maxmemb;
		unsigned short		modes;
		std::list<Member>	membs;
		std::list<&User>	invited;
		std::string			passwd;

	public:
		Channel(const User &exec, const std::string &name);
		// --------------------
		// getters and printers
		const std::string &getName(void) const;
		const std::string &getTopic(void) const;
		unsigned int &getMaxMemb(void) const;
		unsigned short &getModes(void) const;
		const std::string printMemb(void) const;
		// ---------------------
		// command functions
		bool	join(const User &exec);
		bool	kick(const Member &exec, const Member &target);
		bool	message(const Member &exec);
		bool	setTopic(const Member &exec, const std::string &topic);
		//------------------------------------------------
		bool	mode(const Member &exec, const short modes);
		bool	modeInvite(const Member &exec, const &Member);
		bool	modeTopic(const Member &exec, const size_t len);
		bool	modeKey(const Member &exec, const std::string key);
		bool	modeOperator(const Member &exec, const Member &target);
		bool	modeMembMax(const Member &exec, unsigned int num);
};

#endif
