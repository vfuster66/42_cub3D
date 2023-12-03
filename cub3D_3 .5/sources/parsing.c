/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecullier <ecullier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 16:18:09 by vfuster-          #+#    #+#             */
/*   Updated: 2023/12/03 14:40:44 by ecullier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"
#include "../includes/color.h"

// Parsing configuration
int validate_resolution(const char *resolution_str)
{
	char *endptr;
	long width, height;

	width = strtol(resolution_str, &endptr, 10);
	if (resolution_str == endptr)
		return (0);
	while (*endptr == ' ') endptr++;
	height = strtol(endptr, &endptr, 10);
	if (*endptr != '\0')
		return (0);
	if (width <= 0 || height <= 0 || width > MAX_WIDTH || height > MAX_HEIGHT)
	{
		printf("Erreur : valeurs de résolution aberrantes\n");
		return (0);
	}
	return (1);
}

int	validate_rgb(const char *rgb_str, int *rgb_arr)
{
	char	*token;
	char	*str;
	char	*ptr;
	int		i;
	int		value;

	i = 0;
	str = strdup(rgb_str);
	ptr = str;
	while ((token = strtok(ptr, ", ")) != NULL && i < 3)
	{
		value = atoi(token);
		if (value < 0 || value > 255)
		{
			free(str);
			return (0);
		}
		rgb_arr[i++] = value;
		ptr = NULL;
	}
	free(str);
	return (i == 3);
}

int	validate_texture_path(const char *path)
{
	const char	*extension;

	extension = strrchr(path, '.');
	if (extension == NULL || strcmp(extension, ".xpm") != 0)
	{
		return (0);
	}
	return (1);
}

int	process_config_key(char *key, char *value, t_map *map_info)
{
	if (strcmp(key, "WE") == 0 && validate_texture_path(value))
		map_info->west_texture_path = strdup(value);
	else if (strcmp(key, "NO") == 0 && validate_texture_path(value))
		map_info->north_texture_path = strdup(value);
	else if (strcmp(key, "SO") == 0 && validate_texture_path(value))
		map_info->south_texture_path = strdup(value);
	else if (strcmp(key, "EA") == 0 && validate_texture_path(value))
		map_info->east_texture_path = strdup(value);
	else if (strcmp(key, "F") == 0 && validate_rgb(value, map_info->floor_color))
	{
	}
	else if (strcmp(key, "C") == 0 && validate_rgb(value, map_info->ceiling_color))
	{
	}
	else if (strcmp(key, "R") == 0 && validate_resolution(value))
	{
	}
	else
	{
		printf("Clé de configuration non reconnue ou valeur invalide : %s\n", key);
		return (0);
	}
	return (1);
}

int	parse_config_line(char* line, t_map *map_info)
{
	char	*key;
	char	*value;

	key = strtok(line, " \t");
	value = strtok(NULL, "\n");
	if (key != NULL && value != NULL)
	{
	return (process_config_key(key, value, map_info));
	}
	return (0);
}

int	process_config_buffer(char* buffer, t_map *map_info, int *config_completed, int fd)
{
	char	*line;
	char	*saveptr;

	saveptr = NULL;
	buffer[strlen(buffer)] = '\0';
	line = strtok_r(buffer, "\n", &saveptr);
	while (line != NULL && !(*config_completed))
	{
		if (line[0] == '1')
		{
			*config_completed = 1;
			continue ;
		}
		if (!parse_config_line(line, map_info))
		{
			printf("Erreur de parsing à la ligne : %s\n", line);
			close(fd);
			return (0);
		}
		line = strtok_r(NULL, "\n", &saveptr);
	}
	return (1);
}

int	check_config_completion(int config_completed)
{
	if (config_completed)
		return (1);
	else
		return (0);
}


int parse_config_file(const char* file_path, t_map *map_info)
{
	char	buffer[BUFFER_SIZE + 1];
	ssize_t	bytes_read;
	int		fd;
	int		config_completed;

	config_completed = 0;
	fd = open(file_path, O_RDONLY);
	if (fd == -1)
	{
		perror("Erreur lors de l'ouverture du fichier");
		return (0);
	}
	while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0 && !config_completed)
	{
		buffer[bytes_read] = '\0';
		if (!process_config_buffer(buffer, map_info, &config_completed, fd))
		{
			close(fd);
			return (0);
		}
	}
	close(fd);
	return (check_config_completion(config_completed));
}

// Parsing map
int	is_wall_line(const char* line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] != '1')
		{
			return (0);
		}
		i++;
	}
	return (1);
}

int	process_player_position(char *line, t_map *map_info, int line_number)
{
	char	*player_pos;
	char	directions[] = "NSEW";;
	int		i;

	i = 0;
	while (directions[i] != '\0')
	{
		player_pos = strchr(line, directions[i]);
		if (player_pos != NULL)
		{
			if (map_info->player_direction != 0)
			{
				printf("Erreur : Plusieurs positions de départ du joueur trouvées\n");
				return (0);
			}
			map_info->player_x = player_pos - line;
			map_info->player_y = line_number;
			map_info->player_direction = *player_pos;
			*player_pos = '0';
			return (1);
		}
		i++;
	}
	return (1);
}

int	parse_map_line(char* line, t_map *map_info, int line_number)
{
	int		len;

	(void)map_info;
	len = strlen(line);
	if (!process_player_position(line, map_info, line_number))
	{
		return (0);
	}
	if (line[0] != '1' || line[len - 1] != '1')
	{
		return (0);
	}
	return (1);
}

int	process_map_line(char* line, t_map *map_info, int* line_number, int fd)
{
	if (!parse_map_line(line, map_info, *line_number))
	{
		printf("Erreur de parsing à la ligne de carte : %s\n", line);
		close(fd);
		return (0);
	}
	(*line_number)++;
	return (1);
}

int	initialize_map_reading(char* line, t_map *map_info, int* line_number, int* reading_map, int fd)
{
	if (is_wall_line(line))
	{
		*reading_map = 1;
		return (process_map_line(line, map_info, line_number, fd));
	}
	return (1);
}

int	read_and_process_lines(int fd, t_map *map_info, int *reading_map, int *line_number)
{
	char	buffer[BUFFER_SIZE + 1];
	ssize_t	bytes_read;
	char	*line;
	char	*saveptr;

	while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		buffer[bytes_read] = '\0';
		line = strtok_r(buffer, "\n", &saveptr);
		while (line != NULL)
		{
			if (!*reading_map)
			{
				if (!initialize_map_reading(line, map_info, line_number, reading_map, fd))
					return (0);
			}
			else
			{
				if (!process_map_line(line, map_info, line_number, fd))
					return (0);
			}
			line = strtok_r(NULL, "\n", &saveptr);
		}
	}
	return (1);
}

int	parse_map(const char* file_path, t_map *map_info)
{
	int		fd;
	int		reading_map = 0;
	int	line_number = 0;

	fd = open(file_path, O_RDONLY);
	if (fd == -1)
	{
		perror("Erreur lors de l'ouverture du fichier de la carte");
		return (0);
	}
	if (!read_and_process_lines(fd, map_info, &reading_map, &line_number))
	{
		close(fd);
		return (0);
	}
	close(fd);
	return (check_config_completion(reading_map));
}

int parse_file(const char* file_path, t_map *map_info)
{
	printf("Début du parsing du fichier: %s\n", file_path);

	if (!parse_config_file(file_path, map_info))
	{
		printf("Erreur lors du parsing de la configuration\n");
		return 0;
	}
	if (!parse_map(file_path, map_info))
	{
		printf("Erreur lors du parsing de la carte\n");
		return 0;
	}
	printf("Parsing du fichier terminé avec succès\n");
	return 1;
}