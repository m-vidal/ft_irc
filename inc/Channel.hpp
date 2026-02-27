/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:32:05 by marcsilv          #+#    #+#             */
/*   Updated: 2026/02/27 17:38:54 by mvidal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>

class User { };

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
