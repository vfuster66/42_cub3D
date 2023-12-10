/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 21:52:35 by parallels         #+#    #+#             */
/*   Updated: 2023/12/10 20:15:52 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

void calculate_ray_and_deltadist(t_data *data, t_ray *ray, int x)
{
    double camera_x = 2 * x / (double)MAX_WIDTH - 1; 
    ray->ray_dir_x = data->player->dir_x + data->player->plane_x * camera_x;
    ray->ray_dir_y = data->player->dir_y + data->player->plane_y * camera_x;

    ray->delta_dist_x = fabs(1 / ray->ray_dir_x);
    ray->delta_dist_y = fabs(1 / ray->ray_dir_y);
    ray->hit = 0;
}

void calculate_step_and_sidedist(t_data *data, t_ray *ray)
{
    if (ray->ray_dir_x < 0)
    {
        ray->step_x = -1;
        ray->side_dist_x = (data->player->pos_x - ray->map_x) * ray->delta_dist_x;
    }
    else
    {
        ray->step_x = 1;
        ray->side_dist_x = (ray->map_x + 1.0 - data->player->pos_x) * ray->delta_dist_x;
    }
    if (ray->ray_dir_y < 0)
    {
        ray->step_y = -1;
        ray->side_dist_y = (data->player->pos_y - ray->map_y) * ray->delta_dist_y;
    }
    else
    {
        ray->step_y = 1;
        ray->side_dist_y = (ray->map_y + 1.0 - data->player->pos_y) * ray->delta_dist_y;
    }
}

void perform_dda(t_data *data, t_ray *ray)
{
    while (ray->hit == 0)
    {
        // Ajouter des instructions de débogage pour vérifier les valeurs
        printf("ray->map_x: %d, ray->map_y: %d, ray->side: %d, ray->hit: %d\n", ray->map_x, ray->map_y, ray->side, ray->hit);
        
        if (ray->map_x >= 0 && ray->map_x < data->map->width &&
            ray->map_y >= 0 && ray->map_y < data->map->height)
        {
            if (data->map->map[ray->map_y][ray->map_x] > 0)
                ray->hit = 1;
        }
        else
        {
            // Sortir de la boucle si les indices sont en dehors des limites de la carte
            ray->hit = 1;
        }

        if (ray->side_dist_x < ray->side_dist_y)
        {
            ray->side_dist_x += ray->delta_dist_x;
            ray->map_x += ray->step_x;
            ray->side = 0;
        }
        else
        {
            ray->side_dist_y += ray->delta_dist_y;
            ray->map_y += ray->step_y;
            ray->side = 1;
        }
    }
}




void calculate_dist(t_data *data, t_ray *ray)
{
    if (ray->side == 0) ray->perp_wall_dist = (ray->map_x - data->player->pos_x + (1 - ray->step_x) / 2) / ray->ray_dir_x;
    else ray->perp_wall_dist = (ray->map_y - data->player->pos_y + (1 - ray->step_y) / 2) / ray->ray_dir_y;
}

int calculate_height_wall(t_data *data, t_ray *ray)
{
    (void)data;
    int line_height = (int)(MAX_HEIGHT / ray->perp_wall_dist);
    return line_height;
}

void cast_ray(t_data *data, t_ray *ray, int x)
{
    init_ray(ray);
    calculate_ray_and_deltadist(data, ray, x);
    calculate_step_and_sidedist(data, ray);
    perform_dda(data, ray);
    calculate_dist(data, ray);
}
