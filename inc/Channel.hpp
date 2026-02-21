/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@42.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:32:05 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/19 19:41:46 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>
//--------------------------------------------------
#include "inc/numReply.hpp"


class User { };

typedef struct Member {
	User &user;
	bool isoperator;
}

class Channel {

	private:
		std::string			_name;
		std::string			_topic;
		unsigned int		_limit;
		unsigned short		_modes;
		std::list<Member>	_membs;
		std::list<&User>	_invited;
		std::string			_key;

		void serverReply();
        
	public:
		Channel(const User &exec, const std::string &name);
		// --------------------
		// getters and printers
		const std::string &getName(void) const;
		const std::string &getTopic(void) const;
		unsigned int &getMaxMemb(void) const;
		unsigned short &getModes(void) const;
		// ---------------------
		// command functions
		bool	join(const User &exec);
		bool	kick(const Member &exec, const Member &target);
		bool	message(const Member &exec);
		bool	setTopic(const Member &exec, const std::string &topic);
		//------------------------------------------------
		bool	mode(const Member &exec, const short modes);
		void	setMode(unsigned char mode)
		void	unsetMode(unsigned char mode)
		bool	hasMode(unsigned char mode) const
		//---------------------------------------------
		//mode enums (00000) (i k l o t)
		enum ModeFlags {
			INVITE_ONLY = 1 << 0,	// 1  (00001)
			KEY_REQ     = 1 << 1,	// 4  (00010)
			LIMIT_SET   = 1 << 2,	// 16 (00100)
			OPER_ONLY   = 1 << 3,	// 8  (01000)
			TOPIC_PRIV  = 1 << 4	// 2  (10000)
		};
};

#endif
