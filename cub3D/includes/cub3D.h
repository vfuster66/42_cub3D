/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 09:16:25 by vfuster-          #+#    #+#             */
/*   Updated: 2023/12/14 16:02:44 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <fcntl.h>
# include <limits.h>
# include <unistd.h>
# include <ctype.h>
# include <math.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <X11/keysym.h>
# include <X11/X.h>
# include "../libft/libft.h"
# include "../mlx_linux/mlx.h"
# include "colors.h"

/*----------MACROS-----------*/

//window
# define WIN_WIDTH 640
# define WIN_HEIGHT 480

//texture taille
# define TEX_SIZE 64

# ifndef O_DIRECTORY
#  define O_DIRECTORY 00200000
# endif

//mouv
# define MOVESPEED 0.0125
# define ROTSPEED 0.015

//error messages
# define ERROR_USAGE "usage: ./cub3d <path/to/map.cub>"

# define ERROR_FILE_NOT_CUB "Not a .cub file"
# define ERROR_FILE_NOT_XPM "Not an .xpm file"
# define ERROR_FILE_IS_DIRECTORY "Is a directory"
# define ERROR_FLOOR_CEILING "Invalid floor/ceiling RGB color(s)"
# define ERROR_COLOR_FLOOR "Invalid floor RGB color"
# define ERROR_COLOR_CEILING "Invalid ceiling RGB color"
# define ERROR_INVALID_MAP "Map description is either wrong or incomplete"
# define ERROR_INV_LETTER "Invalid character in map"
# define ERROR_NUM_PLAYER "Map has more than one player"
# define ERROR_TEX_RGB_VAL "Invalid RGB value (min: 0, max: 255)"
# define ERROR_TEX_MISSING "Missing texture(s)"
# define ERROR_TEX_INVALID "Invalid texture(s)"
# define ERROR_COLOR_MISSING "Missing color(s)"
# define ERROR_MAP_MISSING "Missing map"
# define ERROR_MAP_TOO_SMALL "Map is not at least 3 lines high"
# define ERROR_MAP_NO_WALLS "Map is not surrounded by walls"
# define ERROR_MAP_LAST "Map is not the last element in file"
# define ERROR_PLAYER_POS "Invalid player position"
# define ERROR_PLAYER_DIR "Map has no player position (expected N, S, E or W)"
# define ERROR_MALLOC "Could not allocate memory"
# define ERROR_MLX_START "Could not start mlx"
# define ERROR_MLX_WIN "Could not create mlx window"
# define ERROR_MLX_IMG "Could not create mlx image"

/*-----------ENUM---------*/

enum e_output
{
	ERROR = 2,
	BREAK = 3,
	CONTINUE = 4
};

enum e_texture_index
{
	NORTH = 0,
	SOUTH = 1,
	EAST = 2,
	WEST = 3
};

/*---------- STRUCTURES ----------*/

//images
typedef struct s_img
{
	void	*img;
	int		*addr;
	int		pixel_bits;
	int		size_line;
	int		endian;
}	t_img;

//textures
typedef struct s_texture
{
	char			*north;
	char			*south;
	char			*west;
	char			*east;
	int				*floor;
	int				*ceiling;
	unsigned long	hex_floor;
	unsigned long	hex_ceiling;
	int				size;
	int				index;
	double			step;
	double			pos;
	int				x;
	int				y;
}	t_texture;

//map
typedef struct s_mapinfo
{
	int			fd;
	int			line_count;
	char		*path;
	char		**file;
	int			height;
	int			width;
	int			index_end_of_map;
}	t_mapinfo;

//raycast
typedef struct s_ray
{
	double	camera_x;
	double	dir_x;
	double	dir_y;
	int		map_x;
	int		map_y;
	int		step_x;
	int		step_y;
	double	sidedist_x;
	double	sidedist_y;
	double	deltadist_x;
	double	deltadist_y;
	double	wall_dist;
	double	wall_x;
	int		side;
	int		line_height;
	int		draw_start;
	int		draw_end;
}	t_ray;

//player
typedef struct s_player
{
	char	dir;
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	int		has_moved;
	int		move_x;
	int		move_y;
	int		rotate;
}	t_player;

//data
typedef struct s_data
{
	void		*mlx;
	void		*win;
	int			win_height;
	int			win_width;
	t_mapinfo	mapinfo;
	char		**map;
	t_player	player;
	t_ray		ray;
	int			**texture_pixels;
	int			**textures;
	t_texture	texinfo;
}	t_data;

/*-------- FONCTIONS ----------*/

/* init/init.c */
void	init_data(t_data *data);
void	init_img_clean(t_img *img);
void	init_ray(t_ray *ray);

/* init/init_mlx.c */
void	init_mlx(t_data *data);
void	init_img(t_data *data, t_img *image, int width, int height);
void	init_texture_img(t_data *data, t_img *image, char *path);

/* init/init_textures.c */
void	init_textures(t_data *data);
void	init_texinfo(t_texture *textures);

/* parsing/check.c */
int		check_file(char *arg, bool cub);

/* parsing/parsing.c */
void	parse_data(char *path, t_data *data);

/* parsing/get_file_data.c */
int		get_file_data(t_data *data, char **map);

/* parsing/fill_color_textures.c */
int		fill_color_textures(t_data *data, t_texture *textures,
			char *line, int j);

/* parsing/game_map.c */
int		create_map(t_data *data, char **map, int i);

/* parsing/check_textures.c */
int		check_textures_validity(t_data *data, t_texture *textures);

/* parsing/check_map.c */
int		check_map(t_data *data, char **map_tab);

/* parsing/check_size_map.c */
int		check_map_sides(t_mapinfo *map, char **map_tab);

/* parsing/utils_p.c */
int		is_a_white_space(char c);
size_t	find_biggest_len(t_mapinfo *map, int i);

/* draw/draw.c */
int		render(t_data *data);
void	render_images(t_data *data);

/* draw/raycast.c */
int		raycasting(t_player *player, t_data *data);

/* draw/textures.c */
void	init_texture_pixels(t_data *data);
void	update_texture_pixels(t_data *data, t_texture *tex, t_ray *ray, int x);

/* draw/utils_img.c */
void	set_image_pixel(t_img *image, int x, int y, int color);

/* mouv/moves.c */
void	listen_for_input(t_data *data);
void	init_player_direction(t_data *data);

/* mouv/player_position.c */
int		validate_move(t_data *data, double new_x, double new_y);

/* mouv/player_move.c */
int		move_player_direction(t_data *data);

/* mouv/player_rotation.c */
int		rotate_player(t_data *data, double rot_dir);

/* free/exit.c */
void	exit_cleanup(t_data *data, int code);
int		exit_game(t_data *data);

/* free/free_data.c */
void	free_tab(void **tab);
int		free_data(t_data *data);

/* error.c */
int		error_msg(char *detail, char *str, int code);
int		error_msg_val(int detail, char *str, int code);

#endif
