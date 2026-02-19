/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numReplies.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atambo <atambo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 15:36:07 by atambo            #+#    #+#             */
/*   Updated: 2026/02/19 15:37:24 by atambo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include inc/numReplies.hpp

namespace Reply {
    /*	This is the generic template for all numerics
		Format: :<server> <numeric> <target> <args> :<message> */
    inline std::string format(
        const std::string& server,
        const std::string& code,
        const std::string& target,
        const std::string& subject,
        const std::string& message
    ) {
        std::string res = ":" + server + " " + code + " " + target;
        if (!subject.empty()) res += " " + subject;
        if (!message.empty()) res += " :" + message;
        res += "\r\n";
        return res;
    }
}
