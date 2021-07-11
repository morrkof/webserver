NAME = webserv
CONFIGSRC = ./config/ConfigurationFile.cpp ./config/ConfigurationServer.cpp \
		./config/MapConfigFile.cpp
SRC = ./main.cpp $(CONFIGSRC) ./requestParsing.cpp ./response.cpp ./Websocket.cpp \
	./AutoIndexPage.cpp
OBJ = $(SRC:.cpp=.o)
CFLAGS = -Wall -Wextra -Werror -std=c++98

.PHONY: all clean fclean re

all: $(NAME)
$(NAME): $(OBJ)
	clang++ $(OBJ) $(CFLAGS) -o $(NAME)
%.o: %.cpp
	clang++ $(CFLAGS) -c $< -o $@
clean:
	rm -rf $(OBJ)
fclean: clean
	rm -rf $(NAME)
re: fclean all
