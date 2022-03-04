
NAME=webserv
LIBNAME=weblib.a
CLANG=clang++
AR=ar
FLAGS=
INCLUDES=includes/
SRCS = webserv.cpp
HRCS = includes/webserv.hpp includes/InkSocketIO.hpp includes/InkSocket.hpp includes/InkServerConfig.hpp includes/InkRespond.hpp includes/InkRequest.hpp includes/InkConfigParser.hpp includes/InkCgi.hpp includes/InkAutoIndex.hpp
OBJS = $(SRCS:.cpp=.o)


all: $(NAME)

$(NAME): $(LIBNAME)
	$(CLANG) $(FLAGS) -g -o $(NAME) $(LIBNAME)

$(LIBNAME): $(OBJS)
	$(AR) rc $(LIBNAME) $(OBJS)

$(OBJS): $(SRCS) $(HRCS)
	$(CLANG) $(FLAGS) -c -g -o $@ $< -I $(INCLUDES)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(LIBNAME)
	rm -rf $(NAME)

re: fclean all