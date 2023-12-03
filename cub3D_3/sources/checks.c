/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 16:18:51 by vfuster-          #+#    #+#             */
/*   Updated: 2023/12/01 11:58:07 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

/* VERIFICATIONS */
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

int	is_valid_character(char c)
{
	return (c == '1' || c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

int	validate_single_map_line(char *line, int i, int height, int *player_start_count)
{
	int	width;
	int	j;

	if (line == NULL)
	{
		return (0);
	}
	width = strlen(line);
	j = 0;
	while (j < width)
	{
		if (!is_valid_character(line[j]))
			return (0);
		if (line[j] == 'N' || line[j] == 'S' || line[j] == 'E' || line[j] == 'W')
			(*player_start_count)++;
		if ((i == 0 || i == height - 1 || j == 0 || j == width - 1) && line[j] != '1')
			return (0);
		j++;
	}
	return (1);
}

int	validate_map(t_map *map_info)
{
	int	height;
	int	player_start_count;
	int	i;

	if (map_info == NULL || map_info->map == NULL)
		return (0);
	height = map_info->height;
	player_start_count = 0;
	i = 0;
	while (i < height)
	{
		if (!validate_single_map_line(map_info->map[i], i, height, &player_start_count))
		{
			return (0);
		}
		i++;
	}
	if (player_start_count != 1)
		return (0);
	return (1);
}
