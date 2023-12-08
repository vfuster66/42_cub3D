/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 09:16:25 by vfuster-          #+#    #+#             */
/*   Updated: 2023/12/08 20:36:52 by parallels        ###   ########.fr       */
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
# include <X11/keysym.h>
# include <X11/X.h>
# include "../libft/includes/libft.h"
# include "../mlx_linux/mlx.h"

# define MAX_WIDTH 1024
# define MAX_HEIGHT 768
# define TEXTURE_WIDTH 64
# define TEXTURE_HEIGHT 64
# define CELL_SIZE 10
# define BUFFER_SIZE 1024

# define ESC 65307
# define LEFT 65361
# define RIGHT 65363

# define RESET "0xff0000"

# define ERR_MLX_START "Could not start mlx"
# define ERR_MLX_WIN "Could not create mlx window"
# define ERR_MLX_IMG "Could not create mlx image"

/*---------- STRUCTURES ----------*/

// Texture
typedef struct s_texture
{
	void	*img;
	char	*path;
	char	*addr;
	int		width;
	int		height;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_texture;

// Raycast
typedef struct s_ray
{
	t_texture	*wall_texture;
	double		cameraX;
	double		rayDirX;
	double		rayDirY;
	int			mapX;
	int			mapY;
	double		sideDistX;
	double		sideDistY;
	double		deltaDistX;
	double		deltaDistY;
	double		perpWallDist;
	int			stepX;
	int			stepY;
	int			hit;
	int			side;
}				t_ray;


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

// Image
typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_img;

// Map
typedef struct s_map
{
	char		*north_texture_path;
	char		*south_texture_path;
	char		*west_texture_path;
	char		*east_texture_path;
    t_texture	north_texture;
    t_texture	south_texture;
    t_texture	east_texture;
    t_texture	west_texture;
	t_mlx		*mlx_ref;
	int			width;
	int			height;
	char		**map;
	int			start_x;
	int			start_y;
	int			player_x;
	int			player_y;
	int			player_direction;
	bool		floor_color_set;
	bool		ceiling_color_set;
	int			ceiling_color[3];
	int			floor_color[3];
	bool		config_done;
	char*		resolution_str;
	int			map_height;
}				t_map;

// Player
typedef struct s_player
{
	double	posX;
	double	posY;	
	double	dirX;
	double	dirY;
	double	planeX; 
	double	planeY;
}				t_player;

// Data
typedef struct s_data
{
	t_mlx		*mlx;
	t_map       *map;
	t_player    *player;
	t_img		*img;
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
int			close_window(t_data *data);
int			keyboard_actions(int key, t_data *data);
void		set_window(t_data *data);
/*----------------------------------------------------------*/
// Init
void		init_player(t_player *player, t_map *map_info);
void		init_ray(t_ray *ray);
void		init_img_clean(t_mlx *mlx);
bool		init_texture(void *mlx, t_texture *texture, char *path);
void		init_img(t_data *data, void *mlx, int width, int height);
/*----------------------------------------------------------*/
// Moves
void		move_forward(t_player *player, double move_speed);
void		move_backward(t_player *player, double move_speed);
void		rotate_right(t_player *player, double rot_speed);
void		rotate_left(t_player *player, double rot_speed);
void		move_left(t_player *player, double move_speed);
void		move_right(t_player *player, double move_speed);
int			handle_input(int keycode, t_data *data);
/*----------------------------------------------------------*/
// Raycast
//void		init_step_and_side_dist(t_ray *ray, t_player *player);
//void		perform_dda(t_data *data, t_ray *ray);
void cast_ray(t_data *data, t_player *player, t_ray *ray, int x);
int 		get_texture_color(t_texture *texture, int x, int y);
t_texture 	*choose_wall_texture(t_data *data, t_ray *ray);
void 		draw_wall_texture(t_data *data, t_ray *ray, int x, int wall_height);
void		draw_map_2d(t_data *data);
void 		draw_square(t_data *data, int x, int y, int size, int color);
int 		get_wall_color(t_data *data, int x, int y);

/*----------------------------------------------------------*/
// Clean
void		clean_exit(t_mlx *mlx, t_map *map);
void		cleanup(t_map *map_info, t_mlx *mlx_info);
void		clean_textures(t_map *map);
/*----------------------------------------------------------*/
// Free
void		free_texture_paths(t_map *map_info);
void		free_map_lines(t_map *map_info);
void		free_map(t_map *map_info);
/*----------------------------------------------------------*/
// Error
int	err_msg(char *detail, char *str, int code);
int	err_msg_val(int detail, char *str, int code);
/*----------------------------------------------------------*/
// Principal
int			initialize_mlx_and_window(t_mlx *mlx_info);
bool		check_wall(t_map *map_info, int x, int y);
void		update_game(t_data *data, t_player *player);
/*----------------------------------------------------------*/
// Draw
void my_mlx_pixel_put(t_img *img, int x, int y, int color);
void draw_player(t_data *data);
void draw_floor(t_data *data);
void draw_ceiling(t_data *data);
bool init_texture(void *mlx, t_texture *texture, char *file_path);
bool init_all_textures(t_mlx *mlx_info, t_map *map_info);
void draw_walls(t_data *data);
void render(t_data *data);

#endif