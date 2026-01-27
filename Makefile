CXXFLAGS = -Wextra -Wall -Werror -std=c++98
OBJS = main.cpp
NAME = ircserv
CXX = c++

all:$(NAME)

$(NAME):$(OBJS)
		$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
		rm -f $(OBJS)

fclean: clean
		rm -f $(NAME)

re: fclean all