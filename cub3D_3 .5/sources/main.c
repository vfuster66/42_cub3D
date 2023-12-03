/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecullier <ecullier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 09:02:58 by vfuster-          #+#    #+#             */
/*   Updated: 2023/12/03 15:35:31 by ecullier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"
#include "../includes/color.h"

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

bool init_texture(void *mlx, t_texture *texture, char *path)
{
    texture->img = mlx_xpm_file_to_image(mlx, path, &texture->width, &texture->height);
    if (!texture->img)
        return false;

      texture->addr = mlx_get_data_addr(texture->img, &texture->bits_per_pixel,
	  	&texture->line_length, &texture->endian);
    return true;
}

void	init_img(t_data *data, t_mlx *image, int width, int height)
{
	init_img_clean(image);
	image->img = mlx_new_image(data->mlx, width, height);
	if (image->img == NULL)
		clean_exit(data->mlx, err_msg("mlx", ERR_MLX_IMG, 1));
	image->addr = (int *)mlx_get_data_addr(image->img, &image->bits_per_pixel,
			&image->line_length, &image->endian);
	return ;
}

void	free_texture_paths(t_map *map_info)
{
	if (map_info->north_texture_path != NULL)
	{
		free(map_info->north_texture_path);
		map_info->north_texture_path = NULL;
	}
	if (map_info->south_texture_path != NULL)
	{
		free(map_info->south_texture_path);
		map_info->south_texture_path = NULL;
	}
	if (map_info->west_texture_path != NULL)
	{
		free(map_info->west_texture_path);
		map_info->west_texture_path = NULL;
	}
	if (map_info->east_texture_path != NULL)
	{
		free(map_info->east_texture_path);
		map_info->east_texture_path = NULL;
	}
}

void	free_map(t_map *map_info)
{
	int		i;

	if (map_info == NULL)
		return ;
	free_texture_paths(map_info);
	if (map_info->map != NULL)
	{
		i = 0;
		while (i < map_info->height && map_info->map[i] != NULL)
		{
			free(map_info->map[i]);
			map_info->map[i] = NULL;
			i++;
		}
		free(map_info->map);
		map_info->map = NULL;
	}
	// Si map_info est alloué dynamiquement, ajouter ceci :
	// free(map_info);
}

int	init_mlx(t_mlx *mlx_info)
{
	mlx_info->mlx = mlx_init();
	if (mlx_info->mlx == NULL)
	{
		printf("Error: MLX initialization failed.\n");
		return (0);
	}
	mlx_info->win = mlx_new_window(mlx_info->mlx, MAX_WIDTH, MAX_HEIGHT, "Cub3D");
	if (mlx_info->win == NULL)
	{
		printf("Error: Window creation failed.\n");
		mlx_destroy_display(mlx_info->mlx);
		free(mlx_info->mlx);
		return (0);
	}
	return (1);
}

/*void init_mlx(t_mlx *mlx, int width, int height, char *title)
{
    mlx->mlx = mlx_init();
    if (!mlx->mlx)
    {
        err_msg("MLX initialization failed", NULL, EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    mlx->win = mlx_new_window(mlx->mlx, width, height, title);
    if (!mlx->win)
    {
        err_msg("Window creation failed", NULL, EXIT_FAILURE);
        mlx_destroy_display(mlx->mlx);
        free(mlx->mlx);
        exit(EXIT_FAILURE);
    }

    mlx->img = mlx_new_image(mlx->mlx, width, height);
    if (!mlx->img)
    {
        err_msg("Image creation failed", NULL, EXIT_FAILURE);
        mlx_destroy_window(mlx->mlx, mlx->win);
        mlx_destroy_display(mlx->mlx);
        free(mlx->mlx);
        exit(EXIT_FAILURE);
    }

    mlx->addr = mlx_get_data_addr(mlx->img, &mlx->bits_per_pixel, &mlx->line_length, &mlx->endian);
}*/

void clean_textures(t_map *map)
{
    if (map->north_texture_path && map->north_texture_path->img)
	{
        mlx_destroy_image(map->mlx, map->north_texture_path->img);
    }
    if (map->south_texture_path && map->south_texture_path->img)
	{
        mlx_destroy_image(map->mlx, map->south_texture_path->img);
    }
    if (map->west_texture_path && map->west_texture_path->img)
	{
        mlx_destroy_image(map->mlx, map->west_texture_path->img);
    }
    if (map->east_texture_path && map->east_texture_path->img)
	{
        mlx_destroy_image(map->mlx, map->east_texture_path->img);
    }
}

void	init_player(t_player *player)
{
	player->dirX = '\0';
	player->posX = 0.0;
	player->posY = 0.0;
	player->dirX = 0.0;
	player->dirY = 0.0;
	player->planeX = 0.0;
	player->planeY = 0.0;
}

void clean_exit(t_mlx *mlx, t_map *map)
{
	if(map)
		clean_textures(map);
    if (mlx->img)
        mlx_destroy_image(mlx->mlx, mlx->img);
    if (mlx->win)
        mlx_destroy_window(mlx->mlx, mlx->win);
    if (mlx->mlx)
    {
        mlx_destroy_display(mlx->mlx);
        free(mlx->mlx);
    }
}

//mouve et press-key

void moveForward(t_player *player, double moveSpeed)
{
    player->posX += player->dirX * moveSpeed;
    player->posY += player->dirY * moveSpeed;
}

void moveBackward(t_player *player, double moveSpeed)
{
    player->posX -= player->dirX * moveSpeed;
    player->posY -= player->dirY * moveSpeed;
}

void rotateRight(t_player *player, double rotSpeed)
{
    double oldDirX = player->dirX;
    player->dirX = player->dirX * cos(-rotSpeed) - player->dirY * sin(-rotSpeed);
    player->dirY = oldDirX * sin(-rotSpeed) + player->dirY * cos(-rotSpeed);
    double oldPlaneX = player->planeX;
    player->planeX = player->planeX * cos(-rotSpeed) - player->planeY * sin(-rotSpeed);
    player->planeY = oldPlaneX * sin(-rotSpeed) + player->planeY * cos(-rotSpeed);
}

void rotateLeft(t_player *player, double rotSpeed)
{
    double oldDirX = player->dirX;
    player->dirX = player->dirX * cos(rotSpeed) - player->dirY * sin(rotSpeed);
    player->dirY = oldDirX * sin(rotSpeed) + player->dirY * cos(rotSpeed);
    double oldPlaneX = player->planeX;
    player->planeX = player->planeX * cos(rotSpeed) - player->planeY * sin(rotSpeed);
    player->planeY = oldPlaneX * sin(rotSpeed) + player->planeY * cos(rotSpeed);
}

void moveLeft(t_player *player, double moveSpeed)
{
    player->posX -= player->planeX * moveSpeed;
    player->posY -= player->planeY * moveSpeed;
}

void moveRight(t_player *player, double moveSpeed)
{
    player->posX += player->planeX * moveSpeed;
    player->posY += player->planeY * moveSpeed;
}


int handle_input(int keycode, t_data *data)
{
    const double moveSpeed = 0.1;
    const double rotSpeed = 0.1;

    if (keycode == 'W') moveForward(data->player, moveSpeed);
    if (keycode == 'S') moveBackward(data->player, moveSpeed);
	if (keycode == 'A') moveLeft(data->player, moveSpeed);
	if (keycode == 'D') moveRight(data->player, moveSpeed);
    if (keycode == RIGHT) rotateRight(data->player, rotSpeed);
    if (keycode == LEFT) rotateLeft(data->player, rotSpeed);

    return 0;
}

bool check_wall(t_map *map_info, int x, int y)
{
    // Vérifiez si les coordonnées sont en dehors des limites de la carte
    if (x < 0 || x >= map_info->width || y < 0 || y >= map_info->height)
        return true;  // Considérez les limites comme des murs

    // Retournez vrai si la position correspond à un mur dans la carte
    return (map_info->map[y][x] == '1');
}


void update_game(t_data *data, t_player *player)
{
    const double moveSpeed = 0.1;  // Vitesse de déplacement du joueur

    // Exemple de déplacement vers l'avant
    double newX = player->posX + player->dirX * moveSpeed;
    double newY = player->posY + player->dirY * moveSpeed;

    // Vérification de collision avec les murs
    if (!check_wall(data->map, (int)newX, (int)player->posY))
	{
        player->posX = newX;
    }
    if (!check_wall(data->map, (int)player->posX, (int)newY))
	{
        player->posY = newY;
    }

    // Autres mises à jour (comme les ennemis, les objets, etc.) peuvent être ajoutées ici
}

//raycast
void initializeStepAndSideDist(t_ray *ray, t_player *player)
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

void performDDA(t_data *data, t_ray *ray)
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

void cast_ray(t_data *data, t_player *player, int x, t_ray *ray)
{
    ray->cameraX = 2 * x / (double)MAX_WIDTH - 1; 
    ray->rayDirX = player->dirX + player->planeX * ray->cameraX;
    ray->rayDirY = player->dirY + player->planeY * ray->cameraX;
    ray->mapX = (int)(player->posX);
    ray->mapY = (int)(player->posY);
    ray->deltaDistX = fabs(1 / ray->rayDirX);
    ray->deltaDistY = fabs(1 / ray->rayDirY);
    ray->hit = 0;

    initializeStepAndSideDist(ray, player);
    performDDA(data, ray);

    if (ray->side == 0) 
        ray->perpWallDist = (ray->mapX - player->posX + (1 - ray->stepX) / 2) / ray->rayDirX;
    else 
        ray->perpWallDist = (ray->mapY - player->posY + (1 - ray->stepY) / 2) / ray->rayDirY;
}



int	process_config(char *file_path, t_map *map_info, t_data *data, t_mlx *mlx_info)
{
	if (parse_file(file_path, map_info) == 0)
	{
		printf("Error parsing the map file.\n");
		return 0;
	}
	else
	{
		data->mlx = mlx_info;
		set_image(file_path, mlx_info);
		set_window(data);
		return 1;
	}
}

int main(int ac, char **av)
{
	int		status;
	t_map map_info = {0};
	t_data data;
	t_mlx mlx_info;

	if (check_arguments(ac, av) != 0)
		return (1);

	if (!init_mlx(&mlx_info))
		return (1);

	status = process_config(av[1], &map_info, &data, &mlx_info);
	free_map(&map_info);
	mlx_destroy_window(mlx_info.mlx, mlx_info.win);
	mlx_destroy_display(mlx_info.mlx);
	free(mlx_info.mlx);
	return (status == 0);
}

/*int main(int ac, char **av)
{
    t_map   map_info;
    t_data  data;
    t_mlx   mlx_info;
	t_player player;

    if (check_arguments(ac, av) != 0)
        return (1);

    data.mlx = &mlx_info;
    data.mlx->mlx = mlx_init();
    if (data.mlx->mlx == NULL)
    {
        printf("Error: MLX initialization failed.\n");
        return (1);
    }

    data.mlx->win = mlx_new_window(data.mlx->mlx, MAX_WIDTH, MAX_HEIGHT, "Cub3D");
    if (data.mlx->win == NULL)
    {
        printf("Error: Window creation failed.\n");
        return (1);
    }

    // ... Votre logique de traitement de la carte ...
    if (parse_to_char_array(av[1], &map_info) != 0)
    {
        printf("Error parsing the map file.\n");
        return (1);
    }
    if (validate_map(&map_info) != 0)
    {
        printf("Invalid map.\n");
        free_map(&map_info);
        return (1);
    }
	init_player(&player);
	set_image(av, &mlx_info);
    set_window(&data);
	while (1)
    {
        // Gestion des entrées utilisateur
        handle_input(&data, &player); // Vous devriez écrire cette fonction

        // Mise à jour du jeu
        update_game(&data, &player); // Vous devriez écrire cette fonction

        // Raycasting pour chaque colonne de l'écran
        for (int x = 0; x < WIDTH; x++)
        {
            // Calculer et dessiner le rayon pour la colonne x
            cast_ray(&data, &player, &ray); // Vous devriez écrire cette fonction
        }

        // Mettre à jour l'affichage avec la nouvelle image
        mlx_put_image_to_window(data.mlx->mlx, data.mlx->win, data.mlx->img, 0, 0);
        
        // Vérifier les événements / mettre à jour l'écran / etc...
        mlx_loop_hook(data.mlx->mlx, &main_loop, &data); // Exemple d'utilisation de loop hook
    }

    // Libération des ressources avant la fin
    free_map(&map_info);
    clean_exit(&mlx_info, &map_info);
    return (0);
}
*/