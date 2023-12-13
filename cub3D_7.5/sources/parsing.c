/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecullier <ecullier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 16:18:09 by vfuster-          #+#    #+#             */
/*   Updated: 2023/12/12 22:45:09 by ecullier         ###   ########.fr       */
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

int	handle_texture_path(char *key, char *value, t_map *map_info)
{
	char	**texture_path;

	texture_path = NULL;
	if (strcmp(key, "WE") == 0)
		texture_path = &map_info->west_texture_path;
	else if (strcmp(key, "NO") == 0)
		texture_path = &map_info->north_texture_path;
	else if (strcmp(key, "SO") == 0)
		texture_path = &map_info->south_texture_path;
	else if (strcmp(key, "EA") == 0)
		texture_path = &map_info->east_texture_path;
	if (texture_path != NULL && validate_texture_path(value))
	{
		*texture_path = strdup(value);
		if (*texture_path == NULL)
		{
			printf("Erreur d'allocation pour la texture %s\n", key);
			return (0);
		}
		return (1);
	}
	return (0);
}

int	handle_color(char *key, char *value, t_map *map_info)
{
	int	rgb_arr[3];
	int	i;

	if ((strcmp(key, "F") == 0 || strcmp(key, "C") == 0) && validate_rgb(value, rgb_arr))
	{
		i = 0;
		while (i < 3)
		{
			if (strcmp(key, "F") == 0)
				map_info->floor_color[i] = rgb_arr[i];
			else
				map_info->ceiling_color[i] = rgb_arr[i];
			i++;
		}
		if (strcmp(key, "F") == 0)
			map_info->floor_color_set = true;
		else
			map_info->ceiling_color_set = true;
		return (1);
	}
	return (0);
}

int	handle_texture_and_color(char *key, char *value, t_map *map_info)
{
	if (!handle_texture_path(key, value, map_info) && !handle_color(key, value, map_info))
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
		printf("Traitement de la ligne : %s\n", line);
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
	printf("Ouverture du fichier de configuration '%s'.\n", file_path);
	result = process_lines(fd, map_info);
	close(fd);
	return (result);
}

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

int process_player_position(char *line, t_map *map_info, int line_number)
{
    char *player_pos;
    char directions[] = "NSEW";
    int i;

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
            int line_length = (int)strlen(line);
            if (map_info->player_x == 0 || map_info->player_x == line_length - 1 ||
                line_number == 0 || line_number == map_info->height - 1 ||
                line[map_info->player_x] == '1')
			{
                printf("Erreur : Position du joueur invalide ou sur un mur\n");
                return (0);
            }
            *player_pos = '0';
            return (1);
        }
        i++;
    }
    return 1;
}

int parse_map_line(char* line, t_map *map_info, int line_number)
{
    int len = strlen(line);
    if (!process_player_position(line, map_info, line_number))
    {
        return (0);
    }
    if (line[0] != '1' || line[len - 1] != '1')
    {
        printf("Erreur : la ligne de carte ne commence ou ne finit pas par 1 : %s\n", line);
        return (0);
    }
    return (1);
}


ssize_t	read_file_to_buffer(int fd, char *buffer, size_t buffer_size)
{
	return (read(fd, buffer, buffer_size));
}

int handle_map_line(char *line, t_map *map_info, int *start_reading_map, int *line_number)
{
	printf("Lecture de la ligne de carte #%d : %s\n", *line_number, line);
    if (*start_reading_map)
    {
        if (!parse_map_line(line, map_info, *line_number))
        {
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
static  void print_map_contents(t_map *map_info)
{
    if (map_info == NULL || map_info->map == NULL)
    {
        printf("La carte n'est pas chargée.\n");
        return;
    }

    printf("Contenu de la carte :\n");
    for (int i = 0; i < map_info->height; i++)
    {
        for (int j = 0; j < map_info->width; j++)
	{
            printf("%c", map_info->map[i][j]);
        }
        printf("\n");
    }
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
	{
		print_map_contents(map_info);
		return (1);
	}
	else
		return (0);
}

int	parse_file(const char* file_path, t_map *map_info)
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

int check_map_elements(t_map *map_info)
{
    int i, j;
    char player_dir = '0';
    double player_x = 0.0, player_y = 0.0;
    printf("Début de la vérification des éléments de la carte.\n");
    for (i = 0; map_info->map[i]; i++) {
        for (j = 0; map_info->map[i][j]; j++)
        {
                printf("Vérification de la case [%d][%d]: '%c'\n", i, j, map_info->map[i][j]);
                if (!ft_strchr("10NSEW", map_info->map[i][j]))
                {
                        printf("Erreur: Caractère invalide dans la carte\n");
                        return (0);
                }
                if (ft_strchr("NSEW", map_info->map[i][j]))
                {
                        if (player_dir != '0')
                        {
                                printf("Erreur: Plusieurs positions de départ du joueur trouvées\n");
                                return (0);
                        }
                        player_dir = map_info->map[i][j];
                        player_x = j + 0.5;
                        player_y = i + 0.5;
                        printf("Position du joueur trouvée: %c à [%f, %f]\n", player_dir, player_x, player_y);
                        map_info->map[i][j] = '0';
                 }
        }
    }
    if (player_dir == '0')
    {
        printf("Erreur: Aucune position de départ du joueur trouvée\n");
        return (0);
    }
    // Stockez la position et la direction du joueur dans votre structure map_info
    map_info->player_direction = player_dir;
    map_info->player_x = player_x;
    map_info->player_y = player_y;
    printf("Position et direction du joueur enregistrées.\n");
    return (1);
}

static bool is_dir(char *arg)
{
        printf("Vérification si '%s' est un répertoire...\n", arg);
        int fd = open(arg, O_DIRECTORY);
         if (fd >= 0)
         {
                close(fd);
                printf("'%s' est un répertoire.\n", arg);
                return true;
        }
        printf("'%s' n'est pas un répertoire.\n", arg);
        return false;
}

// Vérifie si le fichier a une extension .cub
static bool is_cub_file(char *arg)
{
    printf("Vérification si '%s' est un fichier .cub...\n", arg);
    size_t len = strlen(arg);
    bool result = (len > 4 && strcmp(arg + len - 4, ".cub") == 0);
    printf("'%s' est %s un fichier .cub.\n", arg, result ? "" : "pas");
    return result;
}


// Vérifie si le fichier spécifié est valide et non un répertoire
int check_file(char *arg, bool cub) {
    printf("Vérification du fichier '%s'...\n", arg);

    if (is_dir(arg)) {
        printf("Erreur: '%s' est un répertoire.\n", arg);
        return -1;
    }

    printf("Ouverture du fichier '%s'...\n", arg);
    int fd = open(arg, O_RDONLY);
    if (fd == -1) {
        perror("Erreur lors de l'ouverture du fichier");
        return -1;
    }
    close(fd);

    if (cub && !is_cub_file(arg)) {
        printf("Erreur: '%s' n'est pas un fichier .cub.\n", arg);
        return -1;
    }

    printf("Le fichier '%s' a passé toutes les vérifications.\n", arg);
    return 0; // Succès
}


int check_player_position_is_valid(t_map *map_info)
{
        printf("Vérification de la validité de la position du joueur...\n");

        int i = (int)map_info->player_y;
        int j = (int)map_info->player_x;
        size_t line_length;

    // Vérifiez que la position du joueur n'est pas en dehors de la carte
        line_length = strlen(map_info->map[i]);
        if (i < 0 || i >= map_info->height || j < 0 || line_length <= (size_t)j)
        {
                printf("Erreur: Le joueur est en dehors de la carte.\n");
                return 0;
        }

    // Vérifiez que la position du joueur n'est pas adjacente à un espace vide
        if ((i > 0 && !is_valid_map_position(map_info->map[i - 1][j])) ||
        (i < map_info->height - 1 && !is_valid_map_position(map_info->map[i + 1][j])) ||
        (j > 0 && !is_valid_map_position(map_info->map[i][j - 1])) ||
        (j < (int)line_length - 1 && !is_valid_map_position(map_info->map[i][j + 1])))
        {
                printf("Erreur: Le joueur est à côté d'un espace vide ou invalide.\n");
                return 0;
        }
        return 1; // Position du joueur valide
}


// Implémentez cette fonction pour vérifier si un caractère représente une position valide sur la carte
bool is_valid_map_position(char c)
{
    return c == '1' || c == '0'; // Ajoutez d'autres caractères valides selon les besoins
}

// Fonction auxiliaire pour vérifier si un caractère est un espace blanc
static bool is_whitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
}

// Fonction pour vérifier si la carte se termine correctement
int check_map_is_at_the_end(t_map *map_info)
{
    for (int i = map_info->height; map_info->map[i]; i++)
    {
        for (int j = 0; map_info->map[i][j]; j++)
        {
            if (!is_whitespace(map_info->map[i][j]))
            {
                printf("Erreur: Caractères inattendus après la fin de la carte.\n");
                return 0; // Il y a des caractères non vides après la fin de la carte
            }
        }
    }
    return 1; // La carte se termine correctement
}

