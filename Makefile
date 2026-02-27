# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mvidal <mvidal@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/30 19:00:19 by marcsilv          #+#    #+#              #
#    Updated: 2026/02/27 19:33:18 by mvidal           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ircserv

CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98

SRC_DIR		= src/
OBJ_DIR		= obj/
INC_DIR		= inc/

SRC_FILES	= main.cpp Server.cpp User.cpp ACommand.cpp 
HEADERS		= Server.hpp User.hpp ACommand.hpp Channel.hpp

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
