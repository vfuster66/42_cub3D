/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 12:22:36 by virginie          #+#    #+#             */
/*   Updated: 2023/11/29 09:41:07 by vfuster-         ###   ########.fr       */
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
# include "color.h"
# include "../libft/includes/libft.h"
# include "../mlx_linux/mlx.h"

# define MAX_WIDTH 640
# define MAX_HEIGHT 480
# define BUFFER_SIZE 1024
# define ESC 65307
# define LEFT 65361
# define RIGHT 65363

# define RESET "0xff0000"

# define ERR_MLX_START "Could not start mlx"
# define ERR_MLX_WIN "Could not create mlx window"
# define ERR_MLX_IMG "Could not create mlx image"

typedef struct s_ray
{
    double cameraX;
    double rayDirX;
    double rayDirY;
    int mapX;
    int mapY;
    double sideDistX;
    double sideDistY;
    double deltaDistX;
    double deltaDistY;
    double perpWallDist;
    int stepX;
    int stepY;
    int hit;
    int side;
}               t_ray;


typedef struct s_texture
{
    void    *img;       // Pointeur vers l'image de la texture
    char    *addr;      // Adresse de l'image de la texture
    int     width;      // Largeur de la texture
    int     height;     // Hauteur de la texture
    int     bits_per_pixel; // Bits par pixel pour la texture
    int     line_length;    // Longueur de ligne pour la texture
    int     endian;         // Endian pour la texture
}               t_texture;


// Structure pour stocker les données de la carte
typedef struct  s_map
{
    t_texture   *t_texture;
    char        **map;
    int         width;
    int         height;
    int         player_x;
    int         player_y;
    char        player_dir; 
    t_texture   *north_texture_path;
    t_texture   *south_texture_path;
    t_texture   *west_texture_path;
    t_texture   *east_texture_path;
}               t_map;

// Structure mlx
typedef struct s_mlx
{
    void    *mlx;
    void    *win;
    void	*img;
    char	*title;
    char	*addr;
    int		endian;
    int		bits_per_pixel;
    int		line_length;
    int     win_width;
    int     win_height;
}               t_mlx;

typedef struct s_player
{
    double posX, posY;  // Position du joueur
    double dirX, dirY;  // Direction du joueur
    double planeX, planeY; // Le plan de la caméra (son angle de champ de vision)
}               t_player;


typedef struct s_data
{
    t_mlx       *mlx;
    t_map       *map;
    t_player    *player;

}               t_data;



bool check_missing_textures(t_map *map_info);
bool check_missing_characters(const char *map);
bool check_duplicate_characters(const char *map);
bool check_resolution(int width, int height);
bool contains_non_digit(const char *str);
bool check_resolution_format(const char *resolutionStr);
bool check_wall(t_map *map_info, int x, int y);
bool check_borders(t_map *map_info, int x, int y, int dx, int dy);
bool check_player_recursive(t_map *map_info, int x, int y, int *player_count);
bool check_single_player(t_map *map_info);
bool check_valid_characters(t_map *map_info);
int validate_map(t_map *map_info);
void replace_spaces_with_walls(t_map *map_info, int max_length);
int add_line_to_map(t_map *map_info, const char *line);
bool parse_resolution(char *buffer, t_map *map_info);
bool validate_color_value(int value);
bool parse_and_validate_color_value(char **ptr, int *value);
bool parse_color(char *buffer, int *r, int *g, int *b);
bool process_map_line(char *buffer, t_map *map_info, int fd);
bool parse_north_texture(char *buffer, t_map *map_info);
bool parse_south_texture(char *buffer, t_map *map_info);
bool parse_west_texture(char *buffer, t_map *map_info);
bool parse_east_texture(char *buffer, t_map *map_info);
bool process_texture_line(char *buffer, t_map *map_info);
bool handle_line(char *buffer, t_map *map_info, int fd);
void update_line_counts(char c, int *length, int *max_length, int *line_count);
void process_file_buffer(const char *buffer, ssize_t bytes_read, int *length, int *max_length, int *line_count);
void count_lines_and_max_length(const char *filename, int *max_length, int *line_count);
int store_line_in_map(char *line, t_map *map_info, int *line_index);
int read_line(int fd, char *line, t_map *map_info, int max_length, int *line_index);
int initialize_map_array(t_map *map_info, int line_count);
int open_file_and_allocate_line(const char *filename, char **line, int max_length);
int parse_to_char_array(const char *filename, t_map *map_info);
void free_map(t_map *map_info);
int check_file_accessibility(const char *filename);
int check_arguments(int ac, char **av);

int	keyboard_actions(int key, t_data *data); 
int	close_window(t_data *data);
void	set_window(t_data *data);
void	set_image(char **av, t_mlx *mlx);


//err
int	err_msg(char *detail, char *str, int code);
int	err_msg_val(int detail, char *str, int code);

//init
void	init_img_clean(t_mlx *img);
bool init_texture(void *mlx, t_texture *texture, char *path);
void	init_img(t_data *data, t_mlx *image, int width, int height);
void init_mlx(t_mlx *mlx, int width, int height, char *title);
void clean_textures(t_map *map);
void	init_player(t_player *player);
void clean_exit(t_mlx *mlx, t_map *map);


void update_game(t_data *data, t_player *player);
int handle_input(int keycode, t_data *data);
void cast_ray(t_data *data, t_player *player, int x, t_ray *ray);

#endif