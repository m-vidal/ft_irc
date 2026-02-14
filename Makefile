CXXFLAGS = -Wextra -Wall -Werror -std=c++98
INCS = inc/ACommand.hpp inc/Server.hpp inc/User.hpp
OBJS = main.o src/ACommand.o src/Server.o src/User.o
NAME = ircserv
CXX = c++

all:$(NAME)

$(OBJS): $(INCS)

$(NAME):$(OBJS)
		$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
		rm -f $(OBJS)

fclean: clean
		rm -f $(NAME)

re: fclean all