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