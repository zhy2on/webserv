NAME = webserv
CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -pedantic -fsanitize=address
INCLUDES = -I$(HEADERS_DIRECTORY)

HEADERS_DIRECTORY = ./includes/
HEADERS_LIST =
HEADERS = $(addprefix $(HEADERS_DIRECTORY), $(HEADERS_LIST))

SOURCES_DIRECTORY = ./sources/
SOURCES_LIST =	main.cpp
SOURCES = $(addprefix $(SOURCES_DIRECTORY), $(SOURCES_LIST))

OBJECTS_DIRECTORY = ./objects/
OBJECTS_LIST = $(patsubst %.cpp, %.o, $(SOURCES_LIST))
OBJECTS = $(addprefix $(OBJECTS_DIRECTORY), $(OBJECTS_LIST))

RED = \033[0;31m
BLUE = \033[0;34m
RESET = \033[0m

all: $(NAME)

$(NAME) : $(OBJECTS_DIRECTORY) $(OBJECTS)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJECTS) -o $(NAME)
	@echo "\n$(BLUE)$(NAME) : object files created$(RESET)"
	@echo "$(BLUE)$(NAME) : $(NAME) created$(RESET)"

$(OBJECTS_DIRECTORY) :
	@mkdir -p $(OBJECTS_DIRECTORY)
	@echo "$(BLUE)$(NAME) : $(OBJECTS_DIRECTORY) created$(RESET)"

$(OBJECTS_DIRECTORY)%.o : $(SOURCES_DIRECTORY)%.cpp $(HEADERS)
	@$(CXX) $(CXXFLAGS) -c $(INCLUDES) $< -o $@
	@echo "$(BLUE).$(RESET)\c"

clean:
	@rm -rf $(OBJECTS_DIRECTORY)
	@echo "$(RED)$(NAME) : $(OBJECTS_DIRECTORY) deleted$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)$(NAME) : $(NAME) deleted$(RESET)"

re:
	@$(MAKE) fclean
	@$(MAKE) all

.PHONY: all clean fclean re