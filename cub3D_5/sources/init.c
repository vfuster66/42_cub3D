/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 21:39:16 by parallels         #+#    #+#             */
/*   Updated: 2023/12/10 14:24:08 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

void set_player_direction_and_plane(t_player *player, char direction)
{
    if (direction == 'N')
    {
        player->dirX = 0.0; player->dirY = -1.0;
        player->planeX = 0.66; player->planeY = 0.0;
    }
    else if (direction == 'S')
    {
        player->dirX = 0.0; player->dirY = 1.0;
        player->planeX = -0.66; player->planeY = 0.0;
    }
    else if (direction == 'W')
    {
        player->dirX = -1.0; player->dirY = 0.0;
        player->planeX = 0.0; player->planeY = -0.66;
    }
    else if (direction == 'E')
    {
        player->dirX = 1.0; player->dirY = 0.0;
        player->planeX = 0.0; player->planeY = 0.66;
    }
    else
    {
        printf("Erreur : Direction du joueur invalide ('%c').\n", direction);
        exit(EXIT_FAILURE);
    }
}

void init_player(t_player *player, t_map *map_info)
{
    int i;
    char direction;
    
    i = 0;
    while (i < map_info->height)
    {
        if (!process_player_position(map_info->map[i], map_info, i))
        {
            printf("Erreur : Impossible de trouver ou de traiter la position du joueur dans la carte.\n");
            exit(EXIT_FAILURE);
        }
        i++;
    }
    direction = map_info->player_direction;
    player->posX = map_info->player_x + 0.5;
    player->posY = map_info->player_y + 0.5;
    set_player_direction_and_plane(player, direction);
}

void	init_ray(t_ray *ray)
{
	ray->cameraX = 0;
	ray->rayDirX = 0;
	ray->rayDirY = 0;
	ray->mapX = 0;
	ray->mapY = 0;
	ray->sideDistX = 0;
	ray->sideDistY = 0;
	ray->deltaDistX = 0;
	ray->deltaDistY = 0;
	ray->perpWallDist = 0;
	ray->stepX = 0;
	ray->stepY = 0;
	ray->hit = 0;
	ray->side = 0;
}

void	init_img_clean(t_mlx *mlx)
{
	mlx->img = NULL;
	mlx->addr = NULL;
	mlx->bits_per_pixel = 0;
	mlx->line_length = 0;
	mlx->endian = 0;
}

void init_img(t_data *data, void *mlx, int width, int height)
{
    data->img = (t_img *)malloc(sizeof(t_img));
    if (data->img == NULL)
    {
        printf("Error: Failed to allocate memory for image.\n");
        exit(EXIT_FAILURE);
    }
    data->img->img = mlx_new_image(mlx, width, height);
    if (data->img->img == NULL)
    {
        printf("Error: Image creation failed.\n");
        free(data->img);
        exit(EXIT_FAILURE);
    }
    data->img->addr = mlx_get_data_addr(data->img->img, &data->img->bits_per_pixel,
        &data->img->line_length, &data->img->endian);
    if (data->img->addr == NULL)
    {
        printf("Error: Failed to get data address from image.\n");
        mlx_destroy_image(mlx, data->img->img);
        free(data->img);
        exit(EXIT_FAILURE);
    }
}
