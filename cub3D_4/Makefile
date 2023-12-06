# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vfuster- <vfuster-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/27 12:41:34 by virginie          #+#    #+#              #
#    Updated: 2023/12/05 12:11:58 by vfuster-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address
LDFLAGS = -lmlx -lm -lXext -lX11

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
SRCS := $(wildcard sources/*.c)

# Dossier pour les fichiers objets
OBJ_DIR = objects

# Chemin complet pour les fichiers objets
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

# Règle générique pour la création des fichiers objets
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(LIB_INC) $(MLX_INC) -c $< -o $@

all: compile

compile: $(OBJS)
	@if ! [ -f $(NAME) ] || [ "$(OBJS)" -nt "$(NAME)" ]; then \
		make -s -C $(LIB_PATH); \
		make -s -C $(MLX_PATH); \
		$(CC) $(CFLAGS) $(OBJS) $(LIB_INC) $(LIB_LINK) $(MLX_INC) $(MLX_LINK) $(LDFLAGS) -o $(NAME); \
		echo "✅ Cub3D compiled\n"; \
	else \
		echo "✅ All is up-to-date"; \
	fi

clean:
	@$(RM) -r $(OBJ_DIR)
	@make -s -C $(LIB_PATH) clean
	@make -s -C $(MLX_PATH) clean
	@echo "✅ Cub3D Cleaned\n"

fclean: clean
	@$(RM) $(NAME)
	@make -s -C $(LIB_PATH) fclean
	@make -s -C $(MLX_PATH) clean
	@echo "✅ Cub3D Cleaned\n"

re: fclean all

.PHONY: all clean fclean re bonus
