NAME = main
CC = c++
FLAGS = -std=c++98 -Wall -Werror -Wextra

SRC = src/launcher.cpp \
	  src/Input.cpp \
	  src/Server.cpp \
	  src/Client.cpp

OBJ = src/launcher.o \
	  src/Input.o \
	  src/Server.o \
	  src/Client.o

all:$(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $@

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@

clean: 
	$(RM) $(OBJ)

fclean: clean 
	$(RM) $(NAME)

re: clean all

.PHONY:all clean fclean re
