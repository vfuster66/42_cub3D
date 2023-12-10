/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 18:31:29 by parallels         #+#    #+#             */
/*   Updated: 2023/12/10 20:56:39 by parallels        ###   ########.fr       */
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

    player_x = (int)data->player->pos_x;
    player_y = (int)data->player->pos_y;
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

void draw_walls(t_data *data, t_ray *ray, int x)
{
    int line_height = calculate_height_wall(data, ray);
    int draw_start = -line_height / 2 + MAX_HEIGHT / 2;
    if (draw_start < 0) draw_start = 0;
    int draw_end = line_height / 2 + MAX_HEIGHT / 2;
    if (draw_end >= MAX_HEIGHT) draw_end = MAX_HEIGHT - 1;

    int color;
    if (ray->side == 1) color = 0xBBBBBB;
    else color = 0xFFFFFF;

    for (int y = draw_start; y < draw_end; y++)
    {
        my_mlx_pixel_put(data->img, x, y, color);
    }
}

/*int load_texture(t_texture *texture, t_mlx *mlx)
{
    if (texture == NULL || mlx == NULL || texture->path == NULL)
    {
        printf("Error: Invalid texture or mlx pointer.\n");
        return (0);
    }

    printf("Loading texture from path: %s\n", texture->path);

    texture->img = mlx_xpm_file_to_image(mlx->mlx, texture->path, &texture->width, &texture->height);

    if (texture->img == NULL)
    {
        printf("Failed to load texture from path: %s\n", texture->path);
        return (0);
    }

    // Reste du code pour configurer la texture...

    return (1);
}


void unload_texture(t_texture *texture, t_mlx *mlx)
{
    if (texture->img)
    {
        mlx_destroy_image(mlx->mlx, texture->img);
        texture->img = NULL;
    }
    if (texture->path)
    {
        free(texture->path);
        texture->path = NULL;
    }
    texture->width = 0;
    texture->height = 0;
    texture->bits_per_pixel = 0;
    texture->line_length = 0;
    texture->endian = 0;
    texture->addr = NULL;
}


void draw_walls(t_data *data, t_ray *ray, int x)
{
    t_texture *wall_texture;

    if (ray->side == 0)
    {
        if (ray->ray_dir_x > 0)
            wall_texture = &(data->map->east_texture);
        else
            wall_texture = &(data->map->west_texture);
    }
    else
    {
        if (ray->ray_dir_y > 0)
            wall_texture = &(data->map->south_texture);
        else
            wall_texture = &(data->map->north_texture);
    }

    // Charger la texture ici
    load_texture(wall_texture, data->mlx);

    int line_height = calculate_height_wall(data, ray);
    int draw_start = -line_height / 2 + MAX_HEIGHT / 2;
    if (draw_start < 0) draw_start = 0;
    int draw_end = line_height / 2 + MAX_HEIGHT / 2;
    if (draw_end >= MAX_HEIGHT) draw_end = MAX_HEIGHT - 1;

    for (int y = draw_start; y < draw_end; y++)
    {
        int tex_y = (int)(((y * 256 - MAX_HEIGHT * 128 + line_height * 128) * wall_texture->height) / line_height / 256);
        my_mlx_pixel_put(data->img, x, y, wall_texture->addr[tex_y * wall_texture->width + ray->tex_x]);
    }

    // Décharger la texture ici
    unload_texture(wall_texture, data->mlx);
}*/


void render(t_data *data)
{
    int width;
    int height;
    int clear_color;
    int y;
    int x;

    width = MAX_WIDTH;
    height = MAX_HEIGHT;
    clear_color = 0x000000;
    y = 0;
    while (y < height)
    {
        x = 0;
        while (x < width)
        {
            my_mlx_pixel_put(data->img, x, y, clear_color);
            x++;
        }
        y++;
    }
    draw_floor(data);
    draw_ceiling(data);
    x = 0;
    while (x < MAX_WIDTH)
    {
        t_ray ray;
        cast_ray(data, &ray, x);
        draw_walls(data, &ray, x);
        x++;
    }
    draw_player(data);
    mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->img->img, 0, 0);
}

