/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 00:34:17 by atambo            #+#    #+#             */
/*   Updated: 2026/02/15 00:37:57 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include ../inc/Channel.hpp

Channel(const std::string name){
	this.name = name;
}
~Channel(){}

const std::string &getName(void) const;

void setName(const std::string &user);
void makeoperator(const std::vector<Users>::iterator pos);

private:
std::string	name;
std::vector<User> users;
std::vector<bool> isoperator;


