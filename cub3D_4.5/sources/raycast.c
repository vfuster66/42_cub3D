/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecullier <ecullier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 21:52:35 by parallels         #+#    #+#             */
/*   Updated: 2023/12/06 12:23:18 by ecullier         ###   ########.fr       */
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

//get texture
int get_texture_color(t_texture *texture, int x, int y)
{
    char *pixel;

    pixel = texture->addr + (y * texture->line_length + x * (texture->bits_per_pixel / 8));
    return *(int*)pixel;
}

void draw_pixel(t_data *data, int x, int y, int color)
{
    char    *dst;

    if (x >= 0 && x < MAX_WIDTH && y >= 0 && y < MAX_HEIGHT)
    {
        dst = data->img->addr + (y * data->img->line_length + x * (data->img->bits_per_pixel / 8));
        *(unsigned int*)dst = color;
    }
}

t_texture *choose_wall_texture(t_data *data, t_ray *ray)
{
    t_map *map = data->map;

    // Si le mur est un mur vertical (est ou ouest)
    if (ray->side == 0)
    {
        if (ray->rayDirX > 0)
            return map->west_texture_path; // Mur à l'ouest, face est
        else
            return map->east_texture_path; // Mur à l'est, face ouest
    }
    // Si le mur est un mur horizontal (nord ou sud)
    else
    {
        if (ray->rayDirY > 0)
            return map->north_texture_path; // Mur au nord, face sud
        else
            return map->south_texture_path; // Mur au sud, face nord
    }
}

void draw_wall_texture(t_data *data, t_ray *ray, int x, int wall_height)
{
    int y;
    int tex_x, tex_y;
    double wall_x; // où exactement le mur a été touché
    t_texture *wall_texture;

    if (ray->side == 0) wall_x = data->player->posY + ray->perpWallDist * ray->rayDirY;
    else               wall_x = data->player->posX + ray->perpWallDist * ray->rayDirX;
    wall_x -= floor((wall_x));

    // x coordonnée sur la texture
    tex_x = (int)(wall_x * (double)(TEXTURE_WIDTH));
    if (ray->side == 0 && ray->rayDirX > 0) tex_x = TEXTURE_WIDTH - tex_x - 1;
    if (ray->side == 1 && ray->rayDirY < 0) tex_x = TEXTURE_WIDTH - tex_x - 1;

    for (y = 0; y < wall_height; y++)
    {
        int d = y * 256 - MAX_HEIGHT * 128 + wall_height * 128;  // décalage pour le calcul de tex_y
        tex_y = ((d * TEXTURE_HEIGHT) / wall_height) / 256;
        wall_texture = choose_wall_texture(data, ray); // fonction pour choisir la texture correcte
        int color = get_texture_color(wall_texture, tex_x, tex_y);
        draw_pixel(data, x, y + (MAX_HEIGHT - wall_height) / 2, color); // fonction pour dessiner un pixel
    }
}

int get_wall_color(t_data *data, int x, int y)
{
    char wallType = data->map->map[y][x];

    switch (wallType)
    {
        case '1':
            // Retourne la couleur pour le type de mur '1'
            return 0x808080; // exemple : gris
        case '2':
            // Retourne la couleur pour un autre type de mur
            return 0xFF0000; // exemple : rouge
        // Ajoutez des cas supplémentaires pour d'autres types de murs
        default:
            return 0x000000; // Couleur par défaut si aucun mur n'est trouvé
    }
}


void draw_map_2d(t_data *data)
{
    int x, y;
	
    for (y = 0; y < data->map->height; y++)
    {
        for (x = 0; x < data->map->width; x++)
        {
            int color;
            // Déterminez la couleur en fonction du type de cellule
            if (data->map->map[y][x] == '1')  // '1' représente un mur
            {
                color = get_wall_color(data, x, y); // Couleur spécifique pour un mur
            }
            else  // Espaces vides ou autres éléments
            {
                color = 0x000000; // Couleur par défaut pour les espaces ouverts
            }

            // Dessinez chaque cellule
            draw_square(data, x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE,color);
			
			
        }
    }

    // Dessiner le joueur sur la carte
    draw_square(data, data->player->posX * CELL_SIZE, data->player->posY * CELL_SIZE, CELL_SIZE, 0xFF0000); // Rouge pour le joueur
}

void draw_square(t_data *data, int x, int y, int size, int color)
{
    for (int dy = 0; dy < size; dy++)
    {
        for (int dx = 0; dx < size; dx++)
        {
            draw_pixel(data, x + dx, y + dy, color);
        }
    }
}

