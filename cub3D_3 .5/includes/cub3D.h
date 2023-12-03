/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecullier <ecullier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 09:16:25 by vfuster-          #+#    #+#             */
/*   Updated: 2023/12/03 15:34:52 by ecullier         ###   ########.fr       */
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

/*---------- STRUCTURES ----------*/

//raycast
typedef struct s_ray
{
    double	cameraX;
    double	rayDirX;
    double	rayDirY;
    int		mapX;
    int		mapY;
    double	sideDistX;
    double	sideDistY;
    double	deltaDistX;
    double	deltaDistY;
    double	perpWallDist;
    int		stepX;
    int		stepY;
    int		hit;
    int		side;
}               t_ray;

//texture
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

// Map
typedef struct s_map
{
	t_texture	*north_texture_path;// t_texture   *north_texture_path;
	t_texture	*south_texture_path;//t_texture   *south_texture_path;
	t_texture	*west_texture_path;//t_texture   *west_texture_path;
	t_texture	*east_texture_path;//t_texture   *east_texture_path;
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

//palyer
typedef struct s_player
{
    double posX;
	double posY;  // Position du joueur
    double dirX;
	double dirY;  // Direction du joueur
    double planeX; 
	double planeY; // Le plan de la caméra (son angle de champ de vision)
}               t_player;

// Data
typedef struct s_data
{
	t_mlx		*mlx;
	t_map       *map;
    t_player    *player;
}				t_data;

/*---------- FONCTIONS ----------*/
// Parsing
int			validate_rgb(const char *rgb_str, int *rgb_arr);
int			validate_texture_path(const char *path);
int			parse_config_line(char *line, t_map *map_info);
int			parse_config_file(const char *file_path, t_map *map_info);
int			is_wall_line(const char *line);
int			parse_map_line(char *line, t_map *map_info, int line_number);
int			parse_map(const char *file_path, t_map *map_info);
int			parse_file(const char *file_path, t_map *map_info);
/*-----------------------------------------------------------*/
// Check
int			validate_map(t_map *map_info);
int			check_file_accessibility(const char *filename);
int			check_arguments(int ac, char **av);
/*----------------------------------------------------------*/
// Window
void		set_image(char **av, t_mlx *mlx);
int			close_window(t_data *data);
int			keyboard_actions(int key, t_data *data);
void		set_window(t_data *data);
/*----------------------------------------------------------*/
// Free
void		free_map(t_map *map_info);
void 		clean_exit(t_mlx *mlx, t_map *map);
/*----------------------------------------------------------*/
//init
void		init_img_clean(t_mlx *img);
bool 		init_texture(void *mlx, t_texture *texture, char *path);
void		init_img(t_data *data, t_mlx *image, int width, int height);
//void 		init_mlx(t_mlx *mlx, int width, int height, char *title);
void		init_player(t_player *player);
/*-------------------------------------------------------------*/
//err
int			err_msg(char *detail, char *str, int code);
int			err_msg_val(int detail, char *str, int code);
/*--------------------------------------------------------------*/
//raycast
void		update_game(t_data *data, t_player *player);
int			handle_input(int keycode, t_data *data);
void		cast_ray(t_data *data, t_player *player, int x, t_ray *ray);

#endif