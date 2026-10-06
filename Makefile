NAME		=	ircserv

COMPILER	=	c++
FLAGS		=	-Wall -Wextra -Werror -std=c++98 -I. -c

ifdef DEBUG
FLAGS		+= -DDEBUG -g3
else
FLAGS		+= -O2
endif

OBJECT_DIR	=	.objects

SOURCES		=	$(wildcard *.cpp */*.cpp)
OBJECTS		=	$(patsubst %.cpp,$(OBJECT_DIR)/%.o,$(SOURCES))

all: $(NAME)

$(OBJECT_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@printf "\e[0;32m[+] Compiling %s\e[0m\n" $@
	@$(COMPILER) $(FLAGS) $< -o $@

$(NAME): $(OBJECTS)
	@printf "\e[0;32m[+] Linking %s\e[0m\n" $@
	@$(COMPILER) $^ -o $@

clean:
	@printf "\e[0;31m[+] Removing %s\e[0m\n" $(OBJECTS)
	@rm -rf $(OBJECTS) $(OBJECT_DIR)

fclean: clean
	@printf "\e[0;31m[+] Removing %s\e[0m\n" $(NAME)
	@rm -f $(NAME)

re: fclean $(NAME)
