# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: atambo <atambo@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/30 19:00:19 by marcsilv          #+#    #+#              #
#    Updated: 2026/03/10 12:39:39 by atambo           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ircserv

CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98 -I./inc

SRC_DIR		= src/
OBJ_DIR		= obj/
INC_DIR		= inc/

SRC_FILES	=	main.cpp User.cpp Reply.cpp\
				Server.cpp Server_commands.cpp Server_utils.cpp\
				Channel.cpp Channel_mode.cpp
				
HEADERS		=	Server.hpp User.hpp Channel.hpp

SRC			= $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ			= $(addprefix $(OBJ_DIR), $(SRC_FILES:.cpp=.o))
INC			= $(addprefix $(INC_DIR), $(HEADERS))


all: $(NAME)

$(OBJ) : $(INC)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(INC)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
