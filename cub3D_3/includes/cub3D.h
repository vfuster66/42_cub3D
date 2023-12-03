/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 09:16:25 by vfuster-          #+#    #+#             */
/*   Updated: 2023/12/01 11:59:23 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
#define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <fcntl.h>
# include <limits.h>
# include <unistd.h>
# include <ctype.h>
# include <math.h>
# include "../libft/includes/libft.h"
# include "../mlx_linux/mlx.h"

# define MAX_WIDTH 640
# define MAX_HEIGHT 480
# define BUFFER_SIZE 1024

# define ESC 65307
# define LEFT 65361
# define RIGHT 65363

/*---------- STRUCTURES ----------*/
// Map
typedef struct s_map
{
	char	*north_texture_path;
	char	*south_texture_path;
	char	*west_texture_path;
	char	*east_texture_path;
	int		width;
	int		height;
	char	**map;
	int		start_x;
	int		start_y;
	int		player_x;
	int		player_y;
	int		player_direction;
	bool	floor_color_set;
	bool	ceiling_color_set;
	int		ceiling_color[3];
	int		floor_color[3];
	bool	config_done;
	char*	resolution_str;
	int		map_height;
}				t_map;

// Window
typedef struct s_mlx
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*title;
	char	*addr;
	int		endian;
	int		bits_per_pixel;
	int		line_length;
	int		win_width;
	int		win_height;
}				t_mlx;

// Data
typedef struct s_data
{
	t_mlx	*mlx;
}				t_data;

/*---------- FONCTIONS ----------*/
// Parsing Config
int			validate_resolution(const char *resolution_str);
int			validate_rgb(const char *rgb_str, int *rgb_arr);
int			validate_texture_path(const char *path);
int			handle_texture_and_color(char *key, char *value, t_map *map_info);
int			parse_config_line(char* line, t_map *map_info);
int			read_and_split_lines(int fd, char **line, char **saveptr, int *config_completed);
int			process_lines(int fd, t_map *map_info);
int			parse_config_file(const char* file_path, t_map *map_info);
// Parsing Map
int			is_wall_line(const char* line);
int			process_player_position(char *line, t_map *map_info, int line_number);
int			parse_map_line(char* line, t_map *map_info, int line_number);
ssize_t		read_file_to_buffer(int fd, char *buffer, size_t buffer_size);
int			handle_map_line(char *line, t_map *map_info, int *start_reading_map, int *line_number);
int			process_map_lines(char *buffer, t_map *map_info, int *start_reading_map, int *line_number);
int			open_file_for_parsing(const char *file_path);
int			parse_map(const char* file_path, t_map *map_info);
int			parse_file(const char* file_path, t_map *map_info);
/*-----------------------------------------------------------*/
// Check
int			check_file_accessibility(const char *filename);
int			check_arguments(int ac, char **av);
int			is_valid_character(char c);
int			validate_single_map_line(char *line, int line_number,
				int height, int *player_start_count);
int			validate_map(t_map *map_info);
/*----------------------------------------------------------*/
// Window
void		set_image(char **av, t_mlx *mlx);
int			close_window(t_data *data);
int			keyboard_actions(int key, t_data *data);
void		set_window(t_data *data);
/*----------------------------------------------------------*/
// Free
void		free_texture_paths(t_map *map_info);
void		free_map_lines(t_map *map_info);
void		free_map(t_map *map_info);
/*----------------------------------------------------------*/
// Principal
int			initialize_mlx_and_window(t_mlx *mlx_info);
void		cleanup(t_map *map_info, t_mlx *mlx_info);

#endif
