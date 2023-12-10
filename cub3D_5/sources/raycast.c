/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 21:52:35 by parallels         #+#    #+#             */
/*   Updated: 2023/12/10 15:26:26 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

void cast_ray(t_data *data, t_player *player, t_ray *ray, int x)
{
    double cameraX = 2 * x / (double)MAX_WIDTH - 1;
    ray->rayDirX = player->dirX + player->planeX * cameraX;
    ray->rayDirY = player->dirY + player->planeY * cameraX;

    // Position actuelle du rayon
    ray->mapX = (int)player->posX;
    ray->mapY = (int)player->posY;

    // Longueur du rayon de la position actuelle jusqu'au prochain côté x ou y
    ray->deltaDistX = fabs(1 / ray->rayDirX);
    ray->deltaDistY = fabs(1 / ray->rayDirY);
    ray->hit = 0;

    // Calculer la direction du pas et le pas initial
    if (ray->rayDirX < 0) {
        ray->stepX = -1;
        ray->sideDistX = (player->posX - ray->mapX) * ray->deltaDistX;
    } else {
        ray->stepX = 1;
        ray->sideDistX = (ray->mapX + 1.0 - player->posX) * ray->deltaDistX;
    }
    if (ray->rayDirY < 0) {
        ray->stepY = -1;
        ray->sideDistY = (player->posY - ray->mapY) * ray->deltaDistY;
    } else {
        ray->stepY = 1;
        ray->sideDistY = (ray->mapY + 1.0 - player->posY) * ray->deltaDistY;
    }
    // Boucle DDA pour trouver le mur
    while (ray->hit == 0) {
        // Vérifier si on est hors des limites de la carte
        if (ray->mapX < 0 || ray->mapX >= data->map->width || ray->mapY < 0 || ray->mapY >= data->map->height) {
            ray->hit = 1;
            break;
        }

        // Sauter au prochain carré de la carte dans la direction x ou y
        if (ray->sideDistX < ray->sideDistY) {
            ray->sideDistX += ray->deltaDistX;
            ray->mapX += ray->stepX;
            ray->side = 0;
        } else {
            ray->sideDistY += ray->deltaDistY;
            ray->mapY += ray->stepY;
            ray->side = 1;
        }

        // Vérifier si le rayon a frappé un mur
        if (data->map->map[ray->mapY][ray->mapX] == '1') {
            ray->hit = 1;
        }
    }

    // Calculer la distance projetée sur le plan de la caméra (la distance perpendiculaire au mur)
    if (ray->side == 0) {
        ray->perpWallDist = (ray->mapX - player->posX + (1 - ray->stepX) / 2) / ray->rayDirX;
    } else {
        ray->perpWallDist = (ray->mapY - player->posY + (1 - ray->stepY) / 2) / ray->rayDirY;
    }
}

