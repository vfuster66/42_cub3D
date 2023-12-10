/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 18:31:29 by parallels         #+#    #+#             */
/*   Updated: 2023/12/10 15:58:25 by parallels        ###   ########.fr       */
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
    int player_x;
    int player_y;
    int player_width;
    int player_height;
    int color;
    int y;
    int x;

    player_x = (int)data->player->posX;
    player_y = (int)data->player->posY;
    player_width = 10;
    player_height = 10;
    color = 0xFF0000;
    y = player_y;
    while (y < player_y + player_height)
    {
        x = player_x;
        while (x < player_x + player_width)
        {
            my_mlx_pixel_put(data->img, x, y, color);
            x++;
        }
        y++;
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
    floor_color = (data->map->floor_color[0] << 16)
        | (data->map->floor_color[1] << 8) | data->map->floor_color[2];
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
	ceiling_color = (data->map->ceiling_color[0] << 16)
        | (data->map->ceiling_color[1] << 8) | data->map->ceiling_color[2];
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

int get_wall_color(t_texture *texture, int x, int y)
{
    int pixel_index;
    int red;
    int green;
    int blue;
    int color;
    
    if (x >= 0 && x < texture->width && y >= 0 && y < texture->height)
    {
        pixel_index = (y * texture->line_length) + (x * (texture->bits_per_pixel / 8));
        red = (unsigned char)texture->addr[pixel_index + 2];
        green = (unsigned char)texture->addr[pixel_index + 1];
        blue = (unsigned char)texture->addr[pixel_index];
        color = (red << 16) | (green << 8) | blue;
        return (color);
    }
    return (0x000000);
}

void render(t_data *data)
{
    int width;
    int height;
    int clearColor;
    int y;
    int x;

    width = MAX_WIDTH;
    height = MAX_HEIGHT;
    clearColor = 0x000000;
    y = 0;
    while (y < height)
    {
        x = 0;
        while (x < width)
        {
            my_mlx_pixel_put(data->img, x, y, clearColor);
            x++;
        }
        y++;
    }
    draw_floor(data);
    draw_ceiling(data);
    draw_walls(data);
    draw_player(data);
    mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->img->img, 0, 0);
}
