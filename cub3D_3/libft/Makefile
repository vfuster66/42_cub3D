NAME := libft.a
CC := gcc
FLAGS := -Wall -Wextra -Werror

SRC_DIR := sources
OBJ_DIR := objects
INC_DIR := includes

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(FLAGS) -I$(INC_DIR) -c $< -o $@

RM := rm -f

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)

all: $(NAME)

clean:
	@$(RM) -r $(OBJ_DIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
