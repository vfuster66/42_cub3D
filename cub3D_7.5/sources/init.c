/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecullier <ecullier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 21:39:16 by parallels         #+#    #+#             */
/*   Updated: 2023/12/12 22:36:57 by ecullier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

int is_player_inside_map(t_player *player, t_map *map)
{
    if (player->pos_x < 0 || player->pos_x >= map->width ||
        player->pos_y < 0 || player->pos_y >= map->height)
    {
        return 0; // Le joueur est en dehors de la carte
    }
    return 1; // Le joueur est à l'intérieur de la carte
}

void init_map(t_map *map)
{
    if (map == NULL)
    {
        return;  // Retourner si map_info est NULL
    }

    // Initialiser les chemins de texture à NULL
    map->north_texture_path = NULL;
    map->south_texture_path = NULL;
    map->west_texture_path = NULL;
    map->east_texture_path = NULL;

     // Initialiser le pointeur mlx_ref à NULL
    map->mlx_ref = NULL;

    // Initialiser les dimensions de la carte
    map->width = 0;
    map->height = 0;

    // Initialiser le tableau de la carte à NULL
    map->map = NULL;

    // Initialiser les positions de départ
    map->start_x = 0;
    map->start_y = 0;
    map->player_x = 0;
    map->player_y = 0;

    // Initialiser la direction du joueur
    map->player_direction = 0;  // 0 ou une autre valeur par défaut

    // Initialiser les couleurs du sol et du plafond
    map->floor_color_set = false;
    map->ceiling_color_set = false;
    for (int i = 0; i < 3; i++)
    {
        map->ceiling_color[i] = 0;
        map->floor_color[i] = 0;
    }

    // Autres initialisations
    map->config_done = false;
    map->resolution_str = NULL;
    map->map_height = 0;
}

void	initialize_player(t_player *player)
{
	player->pos_x = 0.0;
	player->pos_y = 0.0;
	player->dir_x = 0.0;
	player->dir_y = 0.0;
	player->plane_x = 0.0;
	player->plane_y = 0.0;
}

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
     // Vérification de la perpendicularité des vecteurs
    double dot_product = player->dir_x * player->plane_x + player->dir_y * player->plane_y;
    const double EPSILON = 1e-6;

    if (fabs(dot_product) < EPSILON)
    {
        // Les vecteurs sont considérés comme perpendiculaires
    }
    else
    {
        // Les vecteurs ne sont pas perpendiculaires
        printf("Erreur : Les vecteurs de direction et de plan ne sont pas perpendiculaires.\n");
        exit(EXIT_FAILURE);
    }
}

void init_player(t_player *player, t_map *map)
{
    int i;
    char direction;

    i = 0;
    while (i < map->height)
    {
        if (!process_player_position(map->map[i], map, i))
        {
            printf("Erreur : Impossible de trouver ou de traiter la position du joueur dans la carte.\n");
            exit(EXIT_FAILURE);
        }
        i++;
    }
    direction = map->player_direction;
    player->pos_x = map->player_x + 0.5;
    player->pos_y = map->player_y + 0.5;
    set_player_direction_and_plane(player, direction);
    if (!is_player_inside_map(player, map))
        printf("Erreur : Le joueur est positionné en dehors de la carte1.\n");
    // Gérez l'erreur comme nécessaire
    printf("Position du joueur: x = %f, y = %f\n", player->pos_x, player->pos_y);

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
