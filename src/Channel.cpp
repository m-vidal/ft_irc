/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 00:34:17 by atambo            #+#    #+#             */
/*   Updated: 2026/02/24 15:23:36 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel(const User &exec, const std::string &name) {
	if (checkServerName(name) == false)
		throw (std::runtime_error());
	this.name = name;
	Member creator = {exec, true};
	this.membs.push_back = creator;
	this.topic = "";
	this.maxmeb = 0;
	this.modes = 0;
	this.passwd = "";
}
