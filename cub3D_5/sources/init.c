/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 21:39:16 by parallels         #+#    #+#             */
/*   Updated: 2023/12/08 20:32:53 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

void find_player_start_position(t_map *map_info, t_player *player)
{
    int y;
    int x;
    char    cell;
    
    y = 0;
    while (y < map_info->height)
    {
        x = 0;
        while (x < map_info->width)
        {
            cell = map_info->map[y][x];
            if (cell == 'N' || cell == 'S' || cell == 'E' || cell == 'W')
            {
                player->posX = x + 0.5;
                player->posY = y + 0.5;
                map_info->player_direction = cell;
                return ;
            }
            x++;
        }
        y++;
    }
}

void init_player(t_player *player, t_map *map_info)
{
    find_player_start_position(map_info, player);
    char direction = map_info->player_direction;

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
        return ;
    }
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

/*bool init_texture(void *mlx, t_texture *texture, char *path)
{

    texture->img = mlx_xpm_file_to_image(mlx, path, &texture->width, &texture->height);
    
    if (!texture->img)
    {
        printf("Failed to load texture from path: %s\n", path);
        return (false);
    }

    texture->addr = mlx_get_data_addr(texture->img, &texture->bits_per_pixel, &texture->line_length, &texture->endian);

    if (!texture->addr)
    {
        printf("Failed to retrieve texture data address\n");
        return (false);
    }

    return (true);
}*/

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
