/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 22:32:13 by parallels         #+#    #+#             */
/*   Updated: 2023/12/06 20:24:51 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

void clean_textures(t_map *map)
{
    if (map->north_texture.img)
    {
        mlx_destroy_image(map->mlx_ref, map->north_texture.img);
    }
    if (map->south_texture.img)
    {
        mlx_destroy_image(map->mlx_ref, map->south_texture.img);
    }
    if (map->west_texture.img)
    {
        mlx_destroy_image(map->mlx_ref, map->west_texture.img);
    }
    if (map->east_texture.img)
    {
        mlx_destroy_image(map->mlx_ref, map->east_texture.img);
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
	exit(1);
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
