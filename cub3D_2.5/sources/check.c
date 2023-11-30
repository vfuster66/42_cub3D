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
