/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 12:24:03 by virginie          #+#    #+#             */
/*   Updated: 2023/11/29 10:15:36 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"
#include "../includes/color.h"

int	err_msg(char *detail, char *str, int code)
{
	ft_putstr_fd("tomato" "cub3D: Error", 2);
	if (detail)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(detail, 2);
	}
	if (str)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(str, 2);
	}
	ft_putstr_fd("\n" RESET, 2);
	return (code);
}

int	err_msg_val(int detail, char *str, int code)
{
	ft_putstr_fd("tomato" "cub3D: Error: ", 2);
	ft_putnbr_fd(detail, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("\n" RESET, 2);
	return (code);
}

void	init_ray(t_ray *ray)
{
	ray->cameraX = 0;
	ray->rayDirX = 0;
	ray->rayDirY = 0;
	ray->mapX = 0;
	ray->mapY = 0;
	ray->sideDistX = 0;
	ray->sideDistY = 0;
	ray->deltaDistX = 0;
	ray->deltaDistY = 0;
	ray->perpWallDist = 0;
	ray->stepX = 0;
	ray->stepY = 0;
	ray->hit = 0;
	ray->side = 0;
}

void	init_img_clean(t_mlx *mlx)
{
	mlx->img = NULL;
	mlx->addr = NULL;
	mlx->bits_per_pixel = 0;
	mlx->line_length = 0;
	mlx->endian = 0;
}

bool init_texture(void *mlx, t_texture *texture, char *path)
{
    texture->img = mlx_xpm_file_to_image(mlx, path, &texture->width, &texture->height);
    if (!texture->img)
        return false;

      texture->addr = mlx_get_data_addr(texture->img, &texture->bits_per_pixel,
	  	&texture->line_length, &texture->endian);
    return true;
}

void	init_img(t_data *data, t_mlx *image, int width, int height)
{
	init_img_clean(image);
	image->img = mlx_new_image(data->mlx, width, height);
	if (image->img == NULL)
		clean_exit(data, err_msg("mlx", ERR_MLX_IMG, 1));
	image->addr = (int *)mlx_get_data_addr(image->img, &image->bits_per_pixel,
			&image->line_length, &image->endian);
	return ;
}

void init_mlx(t_mlx *mlx, int width, int height, char *title)
{
    mlx->mlx = mlx_init();
    if (!mlx->mlx)
    {
        err_msg("MLX initialization failed", NULL, EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    mlx->win = mlx_new_window(mlx->mlx, width, height, title);
    if (!mlx->win)
    {
        err_msg("Window creation failed", NULL, EXIT_FAILURE);
        mlx_destroy_display(mlx->mlx);
        free(mlx->mlx);
        exit(EXIT_FAILURE);
    }

    mlx->img = mlx_new_image(mlx->mlx, width, height);
    if (!mlx->img)
    {
        err_msg("Image creation failed", NULL, EXIT_FAILURE);
        mlx_destroy_window(mlx->mlx, mlx->win);
        mlx_destroy_display(mlx->mlx);
        free(mlx->mlx);
        exit(EXIT_FAILURE);
    }

    mlx->addr = mlx_get_data_addr(mlx->img, &mlx->bits_per_pixel, &mlx->line_length, &mlx->endian);
}

void clean_textures(t_map *map)
{
    if (map->north_texture_path && map->north_texture_path->img)
	{
        mlx_destroy_image(map->mlx, map->north_texture_path->img);
    }
    if (map->south_texture_path && map->south_texture_path->img)
	{
        mlx_destroy_image(map->mlx, map->south_texture_path->img);
    }
    if (map->west_texture_path && map->west_texture_path->img)
	{
        mlx_destroy_image(map->mlx, map->west_texture_path->img);
    }
    if (map->east_texture_path && map->east_texture_path->img)
	{
        mlx_destroy_image(map->mlx, map->east_texture_path->img);
    }
}

void	init_player(t_player *player)
{
	player->dir = '\0';
	player->posX = 0.0;
	player->posY = 0.0;
	player->dirX = 0.0;
	player->dirY = 0.0;
	player->planeX = 0.0;
	player->planeY = 0.0;
}

void clean_exit(t_mlx *mlx, t_map *map)
{
	if(map)
		clean_textures(map);
    if (mlx->img)
        mlx_destroy_image(mlx->mlx, mlx->img);
    if (mlx->win)
        mlx_destroy_window(mlx->mlx, mlx->win);
    if (mlx->mlx)
    {
        mlx_destroy_display(mlx->mlx);
        free(mlx->mlx);
    }
}


bool check_missing_textures(t_map *map_info)
{
    if (map_info->north_texture_path == NULL || map_info->south_texture_path == NULL ||
        map_info->west_texture_path == NULL || map_info->east_texture_path == NULL)
	{
        printf("Error: Missing texture paths in configuration.\n");
        return (false);
    }
    return (true);
}


// Verification des caracteres manquants
bool	check_missing_characters(const char *map)
{
	if	(strchr(map, 'N') == NULL || strchr(map, 'S') == NULL
		|| strchr(map, 'E') == NULL || strchr(map, 'W') == NULL)
	{
		printf("Error: Missing characters in the map.\n");
		return (false);
	}
	return (true);
}

// Verification des caracteres en double
bool	check_duplicate_characters(const char *map)
{
	if (strchr(map, 'N') != strrchr(map, 'N')
		|| strchr(map, 'E') != strrchr(map, 'E')
		|| strchr(map, 'S') != strrchr(map, 'S')
		|| strchr(map, 'W') != strrchr(map, 'W'))
	{
		printf("Error: Duplicate characters in the map.\n");
		return (false);
	}
	return (true);
}

// Verification des valeurs incorrectes de la resolution
bool	check_resolution(int width, int height)
{
	if (width <= 0 || height <= 0 || width > INT_MAX || height > INT_MAX)
	{
		printf("Error: Invalid resolution.\n");
		return (false);
	}
	return (true);
}

// Verification de valeurs non numeriques
bool	contains_non_digit(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (!isdigit(str[i]) && str[i] != ',')
		{
			return (true);
		}
		i++;
	}
	return (false);
}

// Verification du format de resolution
bool	check_resolution_format(const char *resolutionStr)
{
	if (contains_non_digit(resolutionStr))
	{
		printf("Error: Invalid characters in resolution.\n");
		return (false);
	}
	return (true);
}

// Verification des murs
bool	check_wall(t_map *map_info, int x, int y)
{
	if (x < 0 || y < 0 || x >= map_info->width || y >= map_info->height)
		return (true);
	return (map_info->map[y][x] == '1');
}

// Verification des limites de la map
bool	check_borders(t_map *map_info, int x, int y, int dx, int dy)
{
	if (x < 0 || y < 0 || x >= map_info->width || y >= map_info->height)
		return (true);
	if (!check_wall(map_info, x, y))
	{
		printf("Error: Map is not surrounded by walls at (%d, %d).\n", x, y);
		return (false);
	}
	return (check_borders(map_info, x + dx, y + dy, dx, dy));
}

// Fonction récursive pour verifier la presence d'un seul player
bool	check_player_recursive(t_map *map_info, int x, int y, int *player_count)
{
	if (x < 0 || y < 0 || x >= map_info->width || y >= map_info->height)
		return (true);
	if (strchr("NSEW", map_info->map[y][x]))
	{
		(*player_count)++;
		if (*player_count > 1)
		{
			printf("Error: Multiple player start positions found.\n");
			return (false);
		}
		map_info->player_x = x;
		map_info->player_y = y;
		map_info->player_dir = map_info->map[y][x];
		map_info->map[y][x] = '0'; 
	}
	if (x < map_info->width - 1)
		return (check_player_recursive(map_info, x + 1, y, player_count));
	else if (y < map_info->height - 1)
		return (check_player_recursive(map_info, 0, y + 1, player_count));
	return (true);
}

// Verification du player
bool	check_single_player(t_map *map_info)
{
	int	player_count;

	player_count = 0;
	if (!check_player_recursive(map_info, 0, 0, &player_count))
		return (false);
	if (player_count == 0)
	{
		printf("Error: No player start position found.\n");
		return (false);
	}
	return (true);
}

// Verification de la validité des caractères dans la map
bool	check_valid_characters(t_map *map_info)
{
	size_t	x;
	int	y;
	char	c;

	y = 0;
	while (y < map_info->height)
	{
		if (map_info->map[y] == NULL)
		{
        	printf("Error: Null pointer encountered at line %d.\n", y);
        	return (false);
		}
		x = 0;
		while (x < strlen(map_info->map[y]))
		{
			c = map_info->map[y][x];
			if (c != '0' && c != '1' && c != '2' && c != 'N' && c != 'S'
				&& c != 'E' && c != 'W')
			{
				printf
					("Error: Invalid character '%c' at position(%ld, %d).\n"
					 , c, x, y);
				return (false);
			}
			x++;
		}
		y++;
	}
	return (true);
}

// Verification de la validite de la map
int	validate_map(t_map *map_info)
{
	if (!check_borders(map_info, 0, 0, 1, 0)
			|| !check_borders(map_info, 0, map_info->height - 1, 1, 0))
		return (1);
	if (!check_borders(map_info, 0, 0, 0, 1)
			|| !check_borders(map_info, map_info->width - 1, 0, 0, 1))
		return (1);
	if (!check_valid_characters(map_info))
		return (1);
	if (!check_single_player(map_info))
		return (1);
	return (0);
}

// Remplacer les espaces par des murs et ajuster la longueur des lignes
void	replace_spaces_with_walls(t_map *map_info, int max_length)
{
	int	y;
	int	x;
	int	current_length;

	y = 0;
	while (y < map_info->height)
	{
		current_length = strlen(map_info->map[y]);
		map_info->map[y] = realloc(map_info->map[y], max_length + 1);
		if (!map_info->map[y])
		{
			perror("Failed to reallocate memory for map line");
			exit(1);
		}
		x = current_length;
		while (x < max_length)
		{
			map_info->map[y][x] = '1';
			x++;
		}
		map_info->map[y][max_length] = '\0';
		y++;
	}
}

int	add_line_to_map(t_map *map_info, const char *line)
{
	static int	row;

	row = 0;
	if (row >= map_info->height)
		return (1);
	map_info->map[row] = strdup(line);
	if (!map_info->map[row])
		return (1);
	replace_spaces_with_walls(map_info, row);
	row++;
	return (0);
}

// Verification de la resolution
bool	parse_resolution(char *buffer, t_map *map_info)
{
	char	*ptr;

	ptr = buffer + 2;
	map_info->width = strtol(ptr, &ptr, 10);
	map_info->height = strtol(ptr, NULL, 10);
	if (map_info->width <= 0 || map_info->height <= 0
		|| map_info->width > MAX_WIDTH || map_info->height > MAX_HEIGHT)
	{
		printf("Error: Invalid resolution.\n");
		return (false);
	}
	if (strchr(buffer, ',') || contains_non_digit(ptr))
	{
		printf("Error: Resolution contains invalid characters.\n");
		return (false);
	}
	return (true);
}

// Vérification des valeurs de couleur
bool	validate_color_value(int value)
{
	return (value >= 0 && value <= 255);
}

// Conversion et vérification des valeurs de couleur
bool	parse_and_validate_color_value(char **ptr, int *value)
{
	*value = strtol(*ptr, ptr, 10);
	if (**ptr != ',' || !validate_color_value(*value))
	{
		printf("Error: Invalid color values.\n");
		return (false);
	}
	(*ptr)++;
	return (true);
}

// Verification de la couleur
bool	parse_color(char *buffer, int *r, int *g, int *b)
{
	char	*ptr;

	ptr = buffer + 2;
	if (buffer[0] != 'F' && buffer[0] != 'C')
	{
		printf
			("Error: Invalid color identifier '%c'. Use 'F' or 'C'.\n",
			 buffer[0]);
		return (false);
	}
	if (!parse_and_validate_color_value(&ptr, r)
			|| !parse_and_validate_color_value(&ptr, g)
			|| !parse_and_validate_color_value(&ptr, b))
		return (false);
	if (!validate_color_value(*r) || !validate_color_value(*g) || !validate_color_value(*b))
	{
		printf("Error: Invalid color values.\n");
		return (false);
	}
	return (true);
}

bool	process_map_line(char *buffer, t_map *map_info, int fd)
{
	int	r;
	int	g;
	int	b;

	(void)fd;
	if (strncmp(buffer, "R ", 2) == 0)
	{
		if (!parse_resolution(buffer, map_info))
			return (false);
	}
	else if (strncmp(buffer, "F ", 2) == 0 || strncmp(buffer, "C ", 2) == 0)
	{
		if (!parse_color(buffer, &r, &g, &b))
			return (false);
	}
	else if (strchr("012NSEW", buffer[0]))
	{
		if (!add_line_to_map(map_info, buffer))
			return (false);
	}
	return (true);
}

// Verification des textures
bool	parse_north_texture(char *buffer, t_map *map_info)
{
	map_info->north_texture_path = strdup(buffer + 3);
	if (!map_info->north_texture_path
		|| strlen(map_info->north_texture_path) == 0)
	{
		printf("Error allocating memory for north texture.\n");
		return (false);
	}
	return (true);
}


bool	parse_south_texture(char *buffer, t_map *map_info)
{
	map_info->south_texture_path = strdup(buffer + 3);
	if (!map_info->south_texture_path
		|| strlen(map_info->south_texture_path) == 0)
	{
		printf("Error allocating memory for south texture.\n");
		return (false);
	}
	return (true);
}

bool	parse_west_texture(char *buffer, t_map *map_info)
{
	map_info->west_texture_path = strdup(buffer + 3);
	if (!map_info->west_texture_path
		|| strlen(map_info->west_texture_path) == 0)
	{
		printf("Error allocating memory for west texture.\n");
		return (false);
	}
	return (true);
}

bool	parse_east_texture(char *buffer, t_map *map_info)
{
	map_info->east_texture_path = strdup(buffer + 3);
	if (!map_info->east_texture_path
		|| strlen(map_info->east_texture_path) == 0)
	{
		printf("Error allocating memory for east texture.\n");
		return (false);
	}
	return (true);
}

bool	process_texture_line(char *buffer, t_map *map_info)
{
	if (strncmp(buffer, "NO ", 3) == 0)
	{
		if (!parse_north_texture(buffer, map_info))
			return (false);
	}
	else if (strncmp(buffer, "SO ", 3) == 0)
	{
		if (!parse_south_texture(buffer, map_info))
			return (false);
	}
	else if (strncmp(buffer, "WE ", 3) == 0)
	{
		if (!parse_west_texture(buffer, map_info))
			return (false);
	}
	else if (strncmp(buffer, "EA ", 3) == 0)
	{
		if (!parse_east_texture(buffer, map_info))
			return (false);
	}
	return (true);
}

bool	handle_line(char *buffer, t_map *map_info, int fd)
{
	if (!process_map_line(buffer, map_info, fd))
		return (false);
	if (!process_texture_line(buffer, map_info))
		return (false);
	return (true);
}

// Mettre à jour le nombre de lignes et la longueur maximale d'une ligne
void	update_line_counts(char c, int *length, int *max_length, int *line_count)
{
	(*length)++;
	if (c == '\n')
	{
		(*line_count)++;
		if (*length > *max_length)
			*max_length = *length;
		*length = 0;
	}
}

// Traiter le buffer et mettre à jour les compteurs
void	process_file_buffer(const char *buffer, ssize_t bytes_read,
		int *length, int *max_length, int *line_count)
{
	int	i;

	i = 0;
	while (i < bytes_read)
	{
		update_line_counts(buffer[i], length, max_length, line_count);
		i++;
	}
}

// Compter les lignes et trouver la longueur maximale d'une ligne
void	count_lines_and_max_length(const char *filename, int *max_length, int *line_count)
{
	char	buffer[BUFFER_SIZE];
	int		fd;
	ssize_t	bytes_read;
	int		length;

	fd = open(filename, O_RDONLY);
	length = 0;
	*max_length = 0;
	*line_count = 0;
	if (fd == -1)
	{
		perror("Error opening file");
		return ;
	}
	while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
		process_file_buffer(buffer, bytes_read, &length, max_length, line_count);
	if (bytes_read == -1)
		perror("Error reading file");
	close(fd);
}

// Ajouter une ligne
int	store_line_in_map(char *line, t_map *map_info, int *line_index)
{
	map_info->map[*line_index] = strdup(line);
	if (!map_info->map[*line_index])
	{
		perror("Failed to allocate memory for map line");
		return (1);
	}
	(*line_index)++;
	return (0);
}

// Lire une ligne du fichier et l'ajouter au tableau de map
int	read_line(int fd, char *line, t_map *map_info,
		int max_length, int *line_index)
{
	int		char_index;
	ssize_t	bytes_read;

	char_index = 0;
	while ((bytes_read = read(fd, line + char_index, 1)) > 0)
	{
		if (line[char_index] == '\n' || char_index == max_length - 1)
		{
			line[char_index] = '\0';
			if (store_line_in_map(line, map_info, line_index) == 1)
				return (1);
			return (0);
		}
		else
			char_index++;
	}
	if (bytes_read == -1)
	{
		perror("Error reading file");
		return (1);
	}
	return (0);
}


// Initialiser le tableau de map
int	initialize_map_array(t_map *map_info, int line_count)
{
	int	i;
	
	map_info->map = malloc(sizeof(char*) * line_count);
	if (!map_info->map)
	{
		perror("Failed to allocate memory for map");
		return (1);
	}
	map_info->height = line_count;
	i = 0;
	while (i < line_count)
	{
		map_info->map[i] = NULL;
		i++;
	}
	return (0);
}

// Ouvrir le fichier et allouer de la memoire pour la ligne
int	open_file_and_allocate_line(const char *filename, char **line, int max_length)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening file");
		return (-1);
	}
	*line = malloc(sizeof(char) * max_length);
	if (!*line)
	{
		perror("Failed to allocate memory for line");
		close(fd);
		return (-1);
	}
	return (fd);
}

// Parser le fichier dans un tableau de char à double entrée
int	parse_to_char_array(const char *filename, t_map *map_info)
{
	int		max_length;
	int		line_count;
	int		line_index;
	int		fd;
	char	*line;

	count_lines_and_max_length(filename, &max_length, &line_count);
	printf("Lines: %d, Max Length: %d\n", line_count, max_length); 
	if (initialize_map_array(map_info, line_count) != 0)
		return (1);
	fd = open_file_and_allocate_line(filename, &line, max_length);
	if (fd == -1)
		return (1);
	line_index = 0;
	while (line_index < line_count)
	{
		if (read_line(fd, line, map_info, max_length, &line_index) != 0)
		{
			printf("Read line: %s\n", line);
			free(line);
			close(fd);
			return (1);
		}
		line_index++;
	}
	close(fd);
	free(line);
	return (0);
}

// Fonction pour libérer la mémoire du tableau de map
void	free_map(t_map *map_info)
{
	int	i;

	i = 0;
	while (i < map_info->height)
	{
		free(map_info->map[i]);
		i++;
	}
	free(map_info->map);
}

// Vérifier l'accessibilité du fichier
int	check_file_accessibility(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("Error: Unable to open the file");
		return (1);
	}
	close(fd);
	return (0);
}

// Vérifier les arguments
int	check_arguments(int ac, char **av)
{
	char	*extension;

	if (ac != 2 && ac != 3)
	{
		printf("Usage: %s <map_file.cub> [--save]\n", av[0]);
		return (1);
	}
	if (ac == 3 && strcmp(av[2], "--save") != 0)
	{
		printf("Error: Invalid argument. The third argument must be '--save'\n");
		return (1);
	}
	extension = strrchr(av[1], '.');
	if (extension == NULL || strcmp(extension, ".cub") != 0
		|| strlen(extension) != 4)
	{
		printf("Error: The map file must have a '.cub' extension.\n");
		return (1);
	}
	return (check_file_accessibility(av[1]));
}

int process_map_file(const char *filename, t_map *map_info)
{
	if (parse_to_char_array(filename, map_info) != 0)
	{
		printf("Error parsing the map file.\n");
		return (1);
	}
    if (!check_missing_textures(map_info))
	{
        free_map(map_info);
        return (1);
    }
	if (validate_map(map_info) != 0)
	{
		printf("Invalid map.\n");
		free_map(map_info);
		return (1);
	}
	return (0);
}

void	set_image(char **av, t_mlx *mlx)
{
	mlx->mlx = mlx_init();
	if (!(mlx->mlx))
	{
		perror("mlx_init error");
		exit(0);
	}
	mlx->title = ft_strjoin("cub3D: ", av[1]);
	mlx->win = mlx_new_window(mlx->mlx, MAX_WIDTH, MAX_HEIGHT, mlx->title);
	mlx->img = mlx_new_image(mlx->mlx,  MAX_WIDTH, MAX_HEIGHT);
	mlx->addr = mlx_get_data_addr(mlx->img, &mlx->bits_per_pixel,
			&mlx->line_length, &mlx->endian);
}

int	close_window(t_data *data)
{
	mlx_destroy_image(data->mlx->mlx, data->mlx->img);
	mlx_destroy_window(data->mlx->mlx, data->mlx->win);
	mlx_destroy_display(data->mlx->mlx);
	free(data->mlx->mlx);
	exit(0);
}

int	keyboard_actions(int key, t_data *data)
{
	if (key == ESC)
		close_window(data);
	mlx_do_sync(data->mlx->mlx);
	mlx_put_image_to_window(data->mlx->mlx, data->mlx->win,
		data->mlx->img, 0, 0);
	return (0);
}

void set_window(t_data *data)
{
    // Fusionnez la logique de keyboard_actions et handle_input si nécessaire
    mlx_key_hook(data->mlx->win, keyboard_actions, data); // Ajustez cette ligne si nécessaire
	mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->mlx->img, 0, 0);

    // Gestionnaire pour la fermeture de la fenêtre
    mlx_hook(data->mlx->win, 17, 0L, close_window, data);

    // Démarrez la boucle d'événements de MiniLibX
    mlx_loop(data->mlx->mlx);
}

int handle_input(int keycode, t_data *data)
{
    const double moveSpeed = 0.1; // Vitesse de déplacement
    const double rotSpeed = 0.1;  // Vitesse de rotation

    // Déplacement vers l'avant
    if (keycode == 'W')
	{
        data->player->posX += data->player->dirX * moveSpeed;
        data->player->posY += data->player->dirY * moveSpeed;
    }
    // Déplacement vers l'arrière
    if (keycode == 'S')
	{
        data->player->posX -= data->player->dirX * moveSpeed;
        data->player->posY -= data->player->dirY * moveSpeed;
    }
    // Rotation à droite
    if (keycode == RIGHT)
	{
        // Rotation de la direction et du plan de la caméra
        double oldDirX = data->player->dirX;
        data->player->dirX = data->player->dirX * cos(-rotSpeed) - data->player->dirY * sin(-rotSpeed);
        data->player->dirY = oldDirX * sin(-rotSpeed) + data->player->dirY * cos(-rotSpeed);
        double oldPlaneX = data->player->planeX;
        data->player->planeX = data->player->planeX * cos(-rotSpeed) - data->player->planeY * sin(-rotSpeed);
        data->player->planeY = oldPlaneX * sin(-rotSpeed) + data->player->planeY * cos(-rotSpeed);
    }
    // Rotation à gauche
    if (keycode == LEFT)
	{
        // Rotation de la direction et du plan de la caméra
        double oldDirX = data->player->dirX;
        data->player->dirX = data->player->dirX * cos(rotSpeed) - data->player->dirY * sin(rotSpeed);
        data->player->dirY = oldDirX * sin(rotSpeed) + data->player->dirY * cos(rotSpeed);
        double oldPlaneX = data->player->planeX;
        data->player->planeX = data->player->planeX * cos(rotSpeed) - data->player->planeY * sin(rotSpeed);
        data->player->planeY = oldPlaneX * sin(rotSpeed) + data->player->planeY * cos(rotSpeed);
    }

    return 0;
}

void update_game(t_data *data, t_player *player)
{
    const double moveSpeed = 0.1;  // Vitesse de déplacement du joueur

    // Exemple de déplacement vers l'avant
    double newX = player->posX + player->dirX * moveSpeed;
    double newY = player->posY + player->dirY * moveSpeed;

    // Vérification de collision avec les murs
    if (!check_wall(data->map, (int)newX, (int)player->posY))
	{
        player->posX = newX;
    }
    if (!check_wall(data->map, (int)player->posX, (int)newY))
	{
        player->posY = newY;
    }

    // Autres mises à jour (comme les ennemis, les objets, etc.) peuvent être ajoutées ici
}

void cast_ray(t_data *data, t_player *player, int x, t_ray *ray)
{
    // Calcul des coordonnées x dans l'espace caméra
    ray->cameraX = 2 * x / (double)MAX_WIDTH - 1; 
    ray->rayDirX = player->dirX + player->planeX * ray->cameraX;
    ray->rayDirY = player->dirY + player->planeY * ray->cameraX;

    // Position actuelle du rayon
    ray->mapX = (int)(player->posX);
    ray->mapY = (int)(player->posY);

    // Calcul de deltaDistX et deltaDistY
    ray->deltaDistX = fabs(1 / ray->rayDirX);
    ray->deltaDistY = fabs(1 / ray->rayDirY);
    ray->hit = 0; // Mur non atteint initialement

    // Calcul de step et initialisation de sideDist
    if (ray->rayDirX < 0)
	{
        ray->stepX = -1;
        ray->sideDistX = (player->posX - ray->mapX) * ray->deltaDistX;
    }
	else
	{
        ray->stepX = 1;
        ray->sideDistX = (ray->mapX + 1.0 - player->posX) * ray->deltaDistX;
    }
    if (ray->rayDirY < 0)
	{
        ray->stepY = -1;
        ray->sideDistY = (player->posY - ray->mapY) * ray->deltaDistY;
    }
	else
	{
        ray->stepY = 1;
        ray->sideDistY = (ray->mapY + 1.0 - player->posY) * ray->deltaDistY;
    }

    // Effectuer le DDA (Digital Differential Analysis)
    while (ray->hit == 0)
	{
        // Avancer vers le prochain carré de la map en X ou Y
        if (ray->sideDistX < ray->sideDistY)
		{
            ray->sideDistX += ray->deltaDistX;
            ray->mapX += ray->stepX;
            ray->side = 0;
        }
		else
		{
            ray->sideDistY += ray->deltaDistY;
            ray->mapY += ray->stepY;
            ray->side = 1;
        }
        // Vérifier si le rayon a frappé un mur
        if (data->map->map[ray->mapY][ray->mapX] == '1') ray->hit = 1;
    }

    // Calcul de la distance projetée sur le plan de la caméra
    if (ray->side == 0) ray->perpWallDist = (ray->mapX - player->posX + (1 - ray->stepX) / 2) / ray->rayDirX;
    else ray->perpWallDist = (ray->mapY - player->posY + (1 - ray->stepY) / 2) / ray->rayDirY;
}



//main

int main(int ac, char **av)
{
    t_map   map_info;
    t_data  data;
    t_mlx   mlx_info;
	t_player player;

    if (check_arguments(ac, av) != 0)
        return (1);

    data.mlx = &mlx_info;
    data.mlx->mlx = mlx_init();
    if (data.mlx->mlx == NULL)
    {
        printf("Error: MLX initialization failed.\n");
        return (1);
    }

    data.mlx->win = mlx_new_window(data.mlx->mlx, MAX_WIDTH, MAX_HEIGHT, "Cub3D");
    if (data.mlx->win == NULL)
    {
        printf("Error: Window creation failed.\n");
        return (1);
    }

    // ... Votre logique de traitement de la carte ...
    if (parse_to_char_array(av[1], &map_info) != 0)
    {
        printf("Error parsing the map file.\n");
        return (1);
    }
    if (validate_map(&map_info) != 0)
    {
        printf("Invalid map.\n");
        free_map(&map_info);
        return (1);
    }
	init_player(&player);
	set_image(av, &mlx_info);
    set_window(&data);
	while (1)
    {
        // Gestion des entrées utilisateur
        handle_input(&data, &player); // Vous devriez écrire cette fonction

        // Mise à jour du jeu
        update_game(&data, &player); // Vous devriez écrire cette fonction

        // Raycasting pour chaque colonne de l'écran
        for (int x = 0; x < WIDTH; x++)
        {
            // Calculer et dessiner le rayon pour la colonne x
            cast_ray(&data, &player, &ray); // Vous devriez écrire cette fonction
        }

        // Mettre à jour l'affichage avec la nouvelle image
        mlx_put_image_to_window(data.mlx->mlx, data.mlx->win, data.mlx->img, 0, 0);
        
        // Vérifier les événements / mettre à jour l'écran / etc...
        mlx_loop_hook(data.mlx->mlx, &main_loop, &data); // Exemple d'utilisation de loop hook
    }

    // Libération des ressources avant la fin
    free_map(&map_info);
    clean_exit(&mlx_info, &map_info);
    return (0);
}
