/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 21:52:35 by parallels         #+#    #+#             */
/*   Updated: 2023/12/04 19:16:06 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"
#include "../includes/color.h"

//raycast
void	init_step_and_side_dist(t_ray *ray, t_player *player)
{
	if (ray->rayDirX < 0)
	{
		ray->stepX = -1;
		ray->sideDistX = (player->posX - ray->mapX) * ray->deltaDistX;
	}
	else
	{
		ray->stepX = 1;
		ray->sideDistX = (ray->mapX + 1.0 - player->posX) * ray->deltaDistX;
	}
	if (ray->rayDirY < 0)
	{
		ray->stepY = -1;
		ray->sideDistY = (player->posY - ray->mapY) * ray->deltaDistY;
	}
	else
	{
		ray->stepY = 1;
		ray->sideDistY = (ray->mapY + 1.0 - player->posY) * ray->deltaDistY;
	}
}

void	perform_dda(t_data *data, t_ray *ray)
{
	while (ray->hit == 0)
	{
		if (ray->sideDistX < ray->sideDistY)
		{
			ray->sideDistX += ray->deltaDistX;
			ray->mapX += ray->stepX;
			ray->side = 0;
		}
		else
		{
			ray->sideDistY += ray->deltaDistY;
			ray->mapY += ray->stepY;
			ray->side = 1;
		}
		if (data->map->map[ray->mapY][ray->mapX] == '1') ray->hit = 1;
	}
}

void	cast_ray(t_data *data, t_player *player, int x, t_ray *ray)
{
	ray->cameraX = 2 * x / (double)MAX_WIDTH - 1; 
	ray->rayDirX = player->dirX + player->planeX * ray->cameraX;
	ray->rayDirY = player->dirY + player->planeY * ray->cameraX;
	ray->mapX = (int)(player->posX);
	ray->mapY = (int)(player->posY);
	ray->deltaDistX = fabs(1 / ray->rayDirX);
	ray->deltaDistY = fabs(1 / ray->rayDirY);
	ray->hit = 0;
	init_step_and_side_dist(ray, player);
	perform_dda(data, ray);
	if (ray->side == 0) 
		ray->perpWallDist = (ray->mapX - player->posX + (1 - ray->stepX) / 2) / ray->rayDirX;
	else 
		ray->perpWallDist = (ray->mapY - player->posY + (1 - ray->stepY) / 2) / ray->rayDirY;
}
