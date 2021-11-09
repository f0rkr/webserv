
NAME=webserv
LIBNAME=weblib.a
CLANG=clang++
AR=ar
FLAGS=-Wall -Werror -Wextra -std=c++98
INCLUDES=includes/
SRCS = webserv.cpp
HRCS = includes/webserv.hpp includes/InkSocket.hpp includes/InkConfigParser.hpp includes/InkServerConfig.hpp
OBJS = $(SRCS:.cpp=.o)


all: $(NAME)

$(NAME): $(LIBNAME)
	$(CLANG) $(FLAGS) -o $(NAME) $(LIBNAME)

$(LIBNAME): $(OBJS)
	$(AR) rc $(LIBNAME) $(OBJS)

$(OBJS): $(SRCS) $(HRCS)
	$(CLANG) $(FLAGS) -c -o $@ $< -I $(INCLUDES)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(LIBNAME)
	rm -rf $(NAME)

re: fclean all