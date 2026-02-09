# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: atambo <atambo@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/02 10:36:28 by atambo            #+#    #+#              #
#    Updated: 2025/12/09 17:19:42 by atambo           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = btc
CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98

OBJS = main.o BitcoinExchange.o Utils.o
HEADERS = BitcoinExchange.hpp Utils.hpp

all : $(NAME)

$(OBJS) : $(HEADERS)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)
	
re : fclean all

.PHONY: all clean fclean re
