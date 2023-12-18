# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vfuster- <vfuster-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/27 12:41:34 by virginie          #+#    #+#              #
#    Updated: 2023/12/14 16:07:08 by vfuster-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address
LDFLAGS = -lmlx -lm -lXext -lX11
DEPFLAGS = -MMD -MP

# Nom du programme
NAME = cub3D

# Bibliothèques
LIB_PATH = ./libft
LIB_NAME = libft.a
LIB_INC = -I$(LIB_PATH)/includes
LIB_LINK = -L$(LIB_PATH) -lft

# MinilibX path (ajustez selon votre installation)
MLX_PATH = ./mlx_linux
MLX_NAME = libmlx_Linux.a
MLX_INC = -I$(MLX_PATH)
MLX_LINK = -L$(MLX_PATH) -lmlx

# Sources
SRCS := $(wildcard sources/*.c sources/draw/*.c sources/free/*.c sources/init/*.c \
		sources/mouv/*.c sources/parsing/*.c)

# Dossier pour les fichiers objets
OBJ_DIR = objects

# Chemin complet pour les fichiers objets
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

# Règle générique pour la création des fichiers objets
$(OBJ_DIR)/%.o: %.c
	@echo 🔜
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(DEPFLAGS) $(LIB_INC) $(MLX_INC) -c $< -o $@


all: compile

compile: $(OBJS)
	@if [ ! -f $(NAME) ]; then \
		#echo "Compiling $(NAME)..."; \
		$(MAKE) $(NAME) | grep -E "Compiling cub3D|\✅ Cub3D compiled|warning|error" || true; \
	else \
		UPDATED=$$(find $(OBJ_DIR) -newer $(NAME)); \
		if [ -n "$$UPDATED" ]; then \
			echo "Recompiling updated objects..."; \
			$(MAKE) $(NAME) | grep -E "Compiling cub3D|\✅ Cub3D compiled|warning|error" || true; \
		else \
			echo "✅ All is up-to-date"; \
		fi \
	fi

$(NAME): $(OBJS)
	@(make -s -C $(LIB_PATH) 2>&1 && make -s -C $(MLX_PATH) 2>&1 && $(CC) $(CFLAGS) $(OBJS) $(LIB_INC) $(LIB_LINK) $(MLX_INC) $(MLX_LINK) $(LDFLAGS) -o $(NAME) 2>&1) | grep -vE "ar: creating|Entering|Leaving|configure \[info\]"
	@echo "✅ Cub3D compiled\n"



clean:
	@$(MAKE) -s -C $(LIB_PATH) clean | grep -v "configure" || true
	@$(MAKE) -s -C $(MLX_PATH) clean | grep -v "configure" || true
	@$(RM) -r $(OBJ_DIR)
	@echo "✅ Cub3D Cleaned\n"

fclean: clean
	@$(RM) $(NAME)
	@echo "✅ Cub3D Fully Cleaned\n"

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re bonus


-include $(DEPS)

.PHONY: all clean fclean re bonus
