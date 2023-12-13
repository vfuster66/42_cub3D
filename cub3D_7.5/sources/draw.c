/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecullier <ecullier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 18:31:29 by parallels         #+#    #+#             */
/*   Updated: 2023/12/12 21:23:07 by ecullier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

void	my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void draw_floor(t_data *data)
{
    int floor_color;
    int y;
    int x;

    floor_color = (data->map->floor_color[0] << 16)
        | (data->map->floor_color[1] << 8) | data->map->floor_color[2];
    y = MAX_HEIGHT / 2;
    while (y < MAX_HEIGHT)
    {
        x = 0;
        while (x < MAX_WIDTH)
        {
            my_mlx_pixel_put(data->img, x, y, floor_color);
            x++;
        }
        y++;
    }
}

void draw_ceiling(t_data *data)
{
    int ceiling_color;
    int y;
    int x;

    ceiling_color = (data->map->ceiling_color[0] << 16)
        | (data->map->ceiling_color[1] << 8) | data->map->ceiling_color[2];
    y = 0;
    while (y < MAX_HEIGHT / 2)
    {
        x = 0;
        while (x < MAX_WIDTH)
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

void    set_image_pixel(t_img *image, int x, int y, int color)
{
    int pixel;

    pixel = y * (image->line_length / 4) + x;
    image->addr[pixel] = color;
}

int load_texture(t_mlx *mlx, t_texture *texture, char *path)
{
    texture->img = mlx_xpm_file_to_image(mlx->mlx, path, &texture->width, &texture->height);
    if (texture->img == NULL)
        return 0;
    texture->addr = mlx_get_data_addr(texture->img, &texture->bits_per_pixel, &texture->line_length, &texture->endian);
    return 1;
}

int get_pixel_from_texture(t_texture *tex, int x, int y)
{
    int pixel = y * tex->width + x;
    return *(int*)(tex->addr + pixel * 4);
}

void draw_walls(t_data *data, t_ray *ray, int x)
{
    int line_height = calculate_height_wall(data, ray);
    int draw_start = -line_height / 2 + MAX_HEIGHT / 2;
    if (draw_start < 0) draw_start = 0;
    int draw_end = line_height / 2 + MAX_HEIGHT / 2;
    if (draw_end >= MAX_HEIGHT) draw_end = MAX_HEIGHT - 1;

    t_texture *tex;
    if (ray->side == 0)
        tex = (ray->ray_dir_x > 0) ? &data->map->east_texture : &data->map->west_texture;
    else
        tex = (ray->ray_dir_y > 0) ? &data->map->south_texture : &data->map->north_texture;

    // Calcul de la position exacte sur le mur
    double wall_x = (ray->side == 0) ? data->player->pos_y + ray->perp_wall_dist * ray->ray_dir_y : data->player->pos_x + ray->perp_wall_dist * ray->ray_dir_x;
    wall_x -= floor(wall_x);

    // x coordinate on the texture
    int tex_x = (int)(wall_x * (double)tex->width);
    if ((ray->side == 0 && ray->ray_dir_x > 0) || (ray->side == 1 && ray->ray_dir_y < 0))
        tex_x = tex->width - tex_x - 1;

    for (int y = draw_start; y < draw_end; y++)
    {
        // Calcul pour déterminer la position y sur la texture
        int d = y * 256 - MAX_HEIGHT * 128 + line_height * 128;
        int tex_y = ((d * tex->height) / line_height) / 256;
        int color = get_pixel_from_texture(tex, tex_x, tex_y);
        my_mlx_pixel_put(data->img, x, y, color);
    }

}

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
    mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->img->img, 0, 0);
}
