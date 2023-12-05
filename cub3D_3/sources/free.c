/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 09:02:58 by vfuster-          #+#    #+#             */
/*   Updated: 2023/12/04 08:27:28 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"
#include "../includes/color.h"

void	free_texture_paths(t_map *map_info)
{
	if (map_info->north_texture_path != NULL)
	{
		free(map_info->north_texture_path);
		map_info->north_texture_path = NULL;
	}
	if (map_info->south_texture_path != NULL)
	{
		free(map_info->south_texture_path);
		map_info->south_texture_path = NULL;
	}
	if (map_info->west_texture_path != NULL)
	{
		free(map_info->west_texture_path);
		map_info->west_texture_path = NULL;
	}
	if (map_info->east_texture_path != NULL)
	{
		free(map_info->east_texture_path);
		map_info->east_texture_path = NULL;
	}
}

void	free_map_lines(t_map *map_info)
{
	int	i;

	i = 0;
	if (map_info->map != NULL)
	{
		while (i < map_info->height && map_info->map[i] != NULL)
		{
			free(map_info->map[i]);
			map_info->map[i] = NULL;
			i++;
		}
		free(map_info->map);
		map_info->map = NULL;
	}
}

void	free_map(t_map *map_info)
{
	if (map_info == NULL)
	{
		return ;
	}
	free_texture_paths(map_info);
	free_map_lines(map_info);
	free(map_info);
}
