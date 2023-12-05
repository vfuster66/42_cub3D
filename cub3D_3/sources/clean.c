/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 22:32:13 by parallels         #+#    #+#             */
/*   Updated: 2023/12/04 08:27:21 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"
#include "../includes/color.h"

void clean_textures(t_map *map)
{
    if (map->north_texture_path && map->north_texture_path->img)
	{
        mlx_destroy_image(map->mlx_ref, map->north_texture_path->img);
    }
    if (map->south_texture_path && map->south_texture_path->img)
	{
        mlx_destroy_image(map->mlx_ref, map->south_texture_path->img);
    }
    if (map->west_texture_path && map->west_texture_path->img)
	{
        mlx_destroy_image(map->mlx_ref, map->west_texture_path->img);
    }
    if (map->east_texture_path && map->east_texture_path->img)
	{
        mlx_destroy_image(map->mlx_ref, map->east_texture_path->img);
    }
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

void	cleanup(t_map *map_info, t_mlx *mlx_info)
{
	if (mlx_info->title != NULL)
	{
		free(mlx_info->title);
		mlx_info->title = NULL;
	}
	free_map(map_info);
	if (mlx_info->win != NULL)
	{
		mlx_destroy_window(mlx_info->mlx, mlx_info->win);
	}
	if (mlx_info->mlx != NULL)
	{
		mlx_destroy_display(mlx_info->mlx);
		free(mlx_info->mlx);
	}
}
