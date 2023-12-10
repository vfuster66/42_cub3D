/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 21:39:16 by parallels         #+#    #+#             */
/*   Updated: 2023/12/10 19:05:32 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

void set_player_direction_and_plane(t_player *player, char direction)
{
    if (direction == 'N')
    {
        player->dir_x = 0.0; player->dir_y = -1.0;
        player->plane_x = 0.66; player->plane_y = 0.0;
    }
    else if (direction == 'S')
    {
        player->dir_x = 0.0; player->dir_y = 1.0;
        player->plane_x = -0.66; player->plane_y = 0.0;
    }
    else if (direction == 'W')
    {
        player->dir_x = -1.0; player->dir_y = 0.0;
        player->plane_x = 0.0; player->plane_y = -0.66;
    }
    else if (direction == 'E')
    {
        player->dir_x = 1.0; player->dir_y = 0.0;
        player->plane_x = 0.0; player->plane_y = 0.66;
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
    player->pos_x = map_info->player_x + 0.5;
    player->pos_y = map_info->player_y + 0.5;
    set_player_direction_and_plane(player, direction);
}

bool init_texture(void *mlx, t_texture *texture, char *file_path)
{
    texture->img = mlx_xpm_file_to_image(mlx, file_path, &texture->width, &texture->height);
    if (!texture->img)
    {
        printf("Failed to load texture from path: %s\n", file_path);
        return false;
    }

    texture->addr = mlx_get_data_addr(texture->img, &texture->bits_per_pixel, &texture->line_length, &texture->endian);
    if (!texture->addr)
    {
        printf("Failed to retrieve texture data address for path: %s\n", file_path);
        return false;
    }
    return true;
}

bool init_all_textures(t_mlx *mlx_info, t_map *map_info)
{
    if (map_info->north_texture_path && !init_texture(mlx_info->mlx, &map_info->north_texture, map_info->north_texture_path))
    {
        printf("Failed to initialize north texture.\n");
        return false;
    }

    if (map_info->south_texture_path && !init_texture(mlx_info->mlx, &map_info->south_texture, map_info->south_texture_path))
    {
        printf("Failed to initialize south texture.\n");
        return false;
    }

    if (map_info->west_texture_path && !init_texture(mlx_info->mlx, &map_info->west_texture, map_info->west_texture_path))
    {
        printf("Failed to initialize west texture.\n");
        return false;
    }

    if (map_info->east_texture_path && !init_texture(mlx_info->mlx, &map_info->east_texture, map_info->east_texture_path))
    {
        printf("Failed to initialize east texture.\n");
        return false;
    }
    return true;
}

void	init_ray(t_ray *ray)
{
	ray->camera_x = 0;
	ray->ray_dir_x = 0;
	ray->ray_dir_y = 0;
	ray->map_x = 0;
	ray->map_y = 0;
	ray->side_dist_x = 0;
	ray->side_dist_y = 0;
	ray->delta_dist_x = 0;
	ray->delta_dist_y = 0;
	ray->perp_wall_dist = 0;
	ray->step_x = 0;
	ray->step_y = 0;
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
