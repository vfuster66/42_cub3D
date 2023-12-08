/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 16:18:09 by vfuster-          #+#    #+#             */
/*   Updated: 2023/12/08 15:53:36 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

int	validate_resolution(const char *resolution_str)
{
	char	*endptr;
	long	width;
	long	height;

	width = strtol(resolution_str, &endptr, 10);
	if (resolution_str == endptr)
		return (0);
	while (*endptr == ' ')
		endptr++;
	height = strtol(endptr, &endptr, 10);
	if (*endptr != '\0')
		return (0);
	if (width <= 0 || height <= 0
		|| width > MAX_WIDTH || height > MAX_HEIGHT)
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

	if (extension == NULL)
	{
		return (0);
	}
	if (strcmp(extension, ".xpm") != 0)
	{
		return (0);
	}
	return (1);
}


int handle_texture_and_color(char *key, char *value, t_map *map_info)
{
    int rgb_arr[3];
    
    if (strcmp(key, "WE") == 0 && validate_texture_path(value))
    {
        map_info->west_texture_path = strdup(value);
        if (map_info->west_texture_path == NULL)
        {
            printf("Erreur d'allocation pour la texture Ouest\n");
            return (0);
        }
    }
    else if (strcmp(key, "NO") == 0 && validate_texture_path(value))
    {
        map_info->north_texture_path = strdup(value);
        if (map_info->north_texture_path == NULL)
        {
            printf("Erreur d'allocation pour la texture Nord\n");
            return (0);
        }
    }
    else if (strcmp(key, "SO") == 0 && validate_texture_path(value))
    {
        map_info->south_texture_path = strdup(value);
        if (map_info->south_texture_path == NULL)
        {
            printf("Erreur d'allocation pour la texture Sud\n");
            return (0);
        }
    }
    else if (strcmp(key, "EA") == 0 && validate_texture_path(value))
    {
        map_info->east_texture_path = strdup(value);
        if (map_info->east_texture_path == NULL)
        {
            printf("Erreur d'allocation pour la texture Est\n");
            return (0);
        }
    }
    else if (strcmp(key, "F") == 0 && validate_rgb(value, rgb_arr))
    {
        for (int i = 0; i < 3; i++)
        {
            map_info->floor_color[i] = rgb_arr[i];
        }
        map_info->floor_color_set = true;
    }
    else if (strcmp(key, "C") == 0 && validate_rgb(value, rgb_arr))
    {
        for (int i = 0; i < 3; i++)
        {
            map_info->ceiling_color[i] = rgb_arr[i];
        }
        map_info->ceiling_color_set = true;
    }
    else
    {
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
		if (strcmp(key, "R") == 0 && validate_resolution(value))
		{
		}
		else
		{
			return (handle_texture_and_color(key, value, map_info));
		}
		return (1);
	}
	return (0);
}

int	read_and_split_lines(int fd, char **line,
		char **saveptr, int *config_completed)
{
	static char	buffer[BUFFER_SIZE + 1];
	ssize_t		bytes_read;

	if (*saveptr == NULL || **saveptr == '\0')
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
			return (0);
		buffer[bytes_read] = '\0';
		*saveptr = buffer;
	}
	*line = strtok_r(*saveptr, "\n", saveptr);
	if (*line != NULL && (*line)[0] == '1')
	{
		*config_completed = 1;
		return (0);
	}
	return (*line != NULL);
}

int	process_lines(int fd, t_map *map_info)
{
	char	*line;
	char	*saveptr;
	int		config_completed;

	saveptr = NULL;
	config_completed = 0;
	while (read_and_split_lines(fd, &line, &saveptr,
		&config_completed) && !config_completed)
	{
		if (!parse_config_line(line, map_info))
		{
			printf("Erreur de parsing à la ligne : %s\n", line);
			return (0);
		}
	}
	if (config_completed)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

int	parse_config_file(const char* file_path, t_map *map_info)
{
	int	fd;
	int	result;

	fd = open(file_path, O_RDONLY);
	if (fd == -1)
	{
		perror("Erreur lors de l'ouverture du fichier");
		return (0);
	}
	result = process_lines(fd, map_info);
	close(fd);
	return (result);
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
	char	directions[] = "NSEW";
	int		i;

	i = 0;
	while (directions[i] != '\0')
	{
		player_pos = strchr(line, directions[i]);
		if (player_pos != NULL)
		{
			if (map_info->player_direction != 0)
			{
				printf
					("Erreur : Plusieurs positions de départ du joueur trouvées\n");
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
	int	len;

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

ssize_t	read_file_to_buffer(int fd, char *buffer, size_t buffer_size)
{
	return (read(fd, buffer, buffer_size));
}

int	handle_map_line(char *line, t_map *map_info,
		int *start_reading_map, int *line_number)
{
	if (*start_reading_map)
	{
		if (!parse_map_line(line, map_info, *line_number))
		{
			printf
				("Erreur de parsing à la ligne de carte : %s\n",
				line);
			return (0);
		}
		(*line_number)++;
	}
	else
	{
		if (is_wall_line(line))
		{
			*start_reading_map = 1;
			if (!parse_map_line(line, map_info, *line_number))
			{
				printf
					("Erreur de parsing à la première ligne de la carte : %s\n",
					line);
				return (0);
			}
			(*line_number)++;
		}
	}
	return (1);
}

int	process_map_lines(char *buffer, t_map *map_info,
		int *start_reading_map, int *line_number)
{
	char	*line;
	char	*saveptr;

	saveptr = NULL;
	line = strtok_r(buffer, "\n", &saveptr);
	while (line != NULL)
	{
		if (!handle_map_line(line, map_info,
			start_reading_map, line_number))
		{
			return (0);
		}
		line = strtok_r(NULL, "\n", &saveptr);
	}
	return (1);
}

int	open_file_for_parsing(const char *file_path)
{
	int	fd;

	fd = open(file_path, O_RDONLY);
	if (fd == -1)
	{
		perror("Erreur lors de l'ouverture du fichier de la carte");
	}
	return (fd);
}

int	parse_map(const char* file_path, t_map *map_info)
{
	char	buffer[BUFFER_SIZE + 1];
	ssize_t	bytes_read;
	int		fd;
	int		start_reading_map;
	int		line_number;

	start_reading_map = 0;
	line_number = 0;
	fd = open_file_for_parsing(file_path);
	if (fd == -1)
		return (0);
	while ((bytes_read = read_file_to_buffer(fd, buffer, BUFFER_SIZE)) > 0)
	{
		buffer[bytes_read] = '\0';
		if (!process_map_lines(buffer, map_info,
			&start_reading_map, &line_number))
		{
			close(fd);
			return (0);
		}
	}
	close(fd);
	if (start_reading_map)
		return (1);
	else
		return (0);
}

int parse_file(const char* file_path, t_map *map_info)
{
    if (!parse_config_file(file_path, map_info))
    {
        printf("Erreur lors du parsing de la configuration\n");
        return (1);
    }
    if (!parse_map(file_path, map_info))
    {
        printf("Erreur lors du parsing de la carte\n");
        return (1);
    }
    return (0);
}
