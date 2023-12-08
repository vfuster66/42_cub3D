/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 18:31:29 by parallels         #+#    #+#             */
/*   Updated: 2023/12/08 20:43:29 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

void	my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	draw_player(t_data *data)
{
    int player_x = (int)data->player->posX;
    int player_y = (int)data->player->posY;
    int player_width = 10;
    int player_height = 10;

    int color = 0xFF0000;

    for (int y = player_y; y < player_y + player_height; y++)
    {
        for (int x = player_x; x < player_x + player_width; x++)
        {
            my_mlx_pixel_put(data->img, x, y, color);
        }
    }
}

void	draw_floor(t_data *data)
{
	int	width;
	int	height;
	int	floor_color;
	int	y;
	int	x;

	width = MAX_WIDTH;
	height = MAX_HEIGHT;
    floor_color = (data->map->floor_color[0] << 16) | (data->map->floor_color[1] << 8)
			| data->map->floor_color[2];
	y = height / 2;
    while (y < height)
    {
		x = 0;
		while (x < width)
		{
			my_mlx_pixel_put(data->img, x, y, floor_color);
			x++;
		}
		y++;
	}
}

void draw_ceiling(t_data *data)
{
	int	width;
	int	height;
	int	ceiling_color;
	int	y;
	int	x;

	width = MAX_WIDTH;
	height = MAX_HEIGHT;
	ceiling_color = (data->map->ceiling_color[0] << 16) | (data->map->ceiling_color[1] << 8) | data->map->ceiling_color[2];
	y = 0;
	while (y < height / 2)
	{
		x = 0;
		while (x < width)
		{
			my_mlx_pixel_put(data->img, x, y, ceiling_color);
			x++;
		}
		y++;
	}
}

bool init_texture(void *mlx, t_texture *texture, char *file_path)
{
    texture->img = mlx_xpm_file_to_image(mlx, file_path, &texture->width, &texture->height);
    if (!texture->img)
        return false;

    texture->addr = mlx_get_data_addr(texture->img, &texture->bits_per_pixel, &texture->line_length, &texture->endian);
    return true;
}

bool init_all_textures(t_mlx *mlx_info, t_map *map_info)
{
    if (map_info->north_texture_path && !init_texture(mlx_info->mlx, &map_info->north_texture, map_info->north_texture_path))
        return false;
    if (map_info->south_texture_path && !init_texture(mlx_info->mlx, &map_info->south_texture, map_info->south_texture_path))
        return false;
    if (map_info->west_texture_path && !init_texture(mlx_info->mlx, &map_info->west_texture, map_info->west_texture_path))
        return false;
    if (map_info->east_texture_path && !init_texture(mlx_info->mlx, &map_info->east_texture, map_info->east_texture_path))
        return false;
    return true;
}


void draw_walls(t_data *data)
{
    t_ray ray;

    for (int x = 0; x < MAX_WIDTH; x++)
    {
        cast_ray(data, data->player, &ray, x);

        int lineHeight = (int)(MAX_HEIGHT / ray.perpWallDist);
        int drawStart = -lineHeight / 2 + MAX_HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + MAX_HEIGHT / 2;
        if (drawEnd >= MAX_HEIGHT) drawEnd = MAX_HEIGHT - 1;

        // Choisir la texture correcte
        t_texture *wall_texture = ray.side == 0 ? (ray.stepX > 0 ? &data->map->east_texture : &data->map->west_texture)
                                                : (ray.stepY > 0 ? &data->map->south_texture : &data->map->north_texture);

        // Calculer la position X dans la texture
        double wallX; // La position exacte du mur frappé par le rayon
        if (ray.side == 0) wallX = data->player->posY + ray.perpWallDist * ray.rayDirY;
        else wallX = data->player->posX + ray.perpWallDist * ray.rayDirX;
        wallX -= floor(wallX);

        int texX = (int)(wallX * (double)wall_texture->width);
        if (ray.side == 0 && ray.rayDirX > 0) texX = wall_texture->width - texX - 1;
        if (ray.side == 1 && ray.rayDirY < 0) texX = wall_texture->width - texX - 1;

        for (int y = drawStart; y < drawEnd; y++)
        {
            // Calculer la position Y dans la texture
            int d = y * 256 - MAX_HEIGHT * 128 + lineHeight * 128;
            int texY = ((d * wall_texture->height) / lineHeight) / 256;
            int color = ((int *)wall_texture->addr)[texY * wall_texture->width + texX];

            // Dessiner le pixel
            my_mlx_pixel_put(data->img, x, y, color);
        }
    }
}


void render(t_data *data)
{
    // Si vous utilisez un buffer d'image, vous devrez le nettoyer ici
    // ...

    // Dessiner les éléments du jeu
    draw_floor(data);
    draw_ceiling(data);
    draw_walls(data);
    draw_player(data); // Assurez-vous que cette fonction dessine le joueur à la bonne position

    // Mettre à jour l'affichage avec la nouvelle image
    // Si vous utilisez un buffer d'image, utilisez mlx_put_image_to_window
    mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->img->img, 0, 0);

    // Si vous utilisez un double buffering, vous pouvez échanger les buffers ici
    // mlx_do_sync(data->mlx->mlx);
}


