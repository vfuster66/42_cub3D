/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 10:14:47 by ecullier          #+#    #+#             */
/*   Updated: 2023/12/14 15:58:46 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3D.h"

static int	check_map_elements(t_data *data, char **map_tab)
{
	int	i;
	int	j;

	i = 0;
	data->player.dir = '0';
	while (map_tab[i] != NULL)
	{
		j = 0;
		while (map_tab[i][j])
		{
			while (data->map[i][j] == ' ' || data->map[i][j] == '\t'
			|| data->map[i][j] == '\r'
			|| data->map[i][j] == '\v' || data->map[i][j] == '\f')
				j++;
			if (!(ft_strchr("10NSEW", map_tab[i][j])))
				return (error_msg(data->mapinfo.path, ERROR_INV_LETTER, 1));
			if (ft_strchr("NSEW", map_tab[i][j]) && data->player.dir != '0')
				return (error_msg(data->mapinfo.path, ERROR_NUM_PLAYER, 1));
			if (ft_strchr("NSEW", map_tab[i][j]) && data->player.dir == '0')
				data->player.dir = map_tab[i][j];
			j++;
		}
		i++;
	}
	return (0);
}

static int	check_position(t_data *data, char **map_tab)
{
	int	i;
	int	j;

	i = (int)data->player.pos_y;
	j = (int)data->player.pos_x;
	if (ft_strlen(map_tab[i - 1]) < (size_t)j
		|| ft_strlen(map_tab[i + 1]) < (size_t)j
		|| is_a_white_space(map_tab[i][j - 1]) == 0
		|| is_a_white_space(map_tab[i][j + 1]) == 0
		|| is_a_white_space(map_tab[i - 1][j]) == 0
		|| is_a_white_space(map_tab[i + 1][j]) == 0)
		return (1);
	return (0);
}

static int	check_player_position(t_data *data, char **map_tab)
{
	int	i;
	int	j;

	if (data->player.dir == '0')
		return (error_msg(data->mapinfo.path, ERROR_PLAYER_DIR, 1));
	i = 0;
	while (map_tab[i])
	{
		j = 0;
		while (map_tab[i][j])
		{
			if (ft_strchr("NSEW", map_tab[i][j]))
			{
				data->player.pos_x = (double)j + 0.5;
				data->player.pos_y = (double)i + 0.5;
				map_tab[i][j] = '0';
			}
			j++;
		}
		i++;
	}
	if (check_position(data, map_tab) == 1)
		return (error_msg(data->mapinfo.path, ERROR_PLAYER_POS, 1));
	return (0);
}

static int	check_map_is_at_the_end(t_mapinfo *map)
{
	int	i;
	int	j;

	i = map->index_end_of_map;
	while (map->file[i])
	{
		j = 0;
		while (map->file[i][j])
		{
			if (map->file[i][j] != ' ' && map->file[i][j] != '\t'
				&& map->file[i][j] != '\r' && map->file[i][j] != '\n'
				&& map->file[i][j] != '\v' && map->file[i][j] != '\f')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	check_map(t_data *data, char **map_tab)
{
	if (!data->map)
		return (error_msg(data->mapinfo.path, ERROR_MAP_MISSING, 1));
	if (check_map_sides(&data->mapinfo, map_tab) == 1)
		return (error_msg(data->mapinfo.path, ERROR_MAP_NO_WALLS, 1));
	if (data->mapinfo.height < 3)
		return (error_msg(data->mapinfo.path, ERROR_MAP_TOO_SMALL, 1));
	if (check_map_elements(data, map_tab) == 1)
		return (1);
	if (check_player_position(data, map_tab) == 1)
		return (1);
	if (check_map_is_at_the_end(&data->mapinfo) == 1)
		return (error_msg(data->mapinfo.path, ERROR_MAP_LAST, 1));
	return (0);
}
