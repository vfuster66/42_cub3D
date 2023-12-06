/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecullier <ecullier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 09:02:58 by vfuster-          #+#    #+#             */
/*   Updated: 2023/12/06 12:25:42 by ecullier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"
#include "../includes/color.h"

/*int	initialize_mlx_and_window(t_mlx *mlx_info)
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
}*/

/*int initialize_mlx_and_window(t_mlx *mlx_info)
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
}*/

int initialize_mlx_and_window(t_mlx *mlx_info)
{
    printf("Initializing MLX\n"); // Débogage
    mlx_info->mlx = mlx_init();

    if (mlx_info->mlx == NULL)
    {
        printf("Error: MLX initialization failed.\n"); // Débogage
        return (0);
    }
    else
    {
        printf("MLX initialized successfully\n"); // Débogage
    }

    printf("Creating MLX window with width %d and height %d\n", MAX_WIDTH, MAX_HEIGHT); // Débogage
    mlx_info->win = mlx_new_window(mlx_info->mlx, MAX_WIDTH, MAX_HEIGHT, "Cub3D");

    if (mlx_info->win == NULL)
    {
        printf("Error: Window creation failed.\n"); // Débogage
        mlx_destroy_display(mlx_info->mlx);
        free(mlx_info->mlx);
        return (0);
    }
    else
    {
        printf("Window created successfully\n"); // Débogage
    }

    return (1);
}


bool	check_wall(t_map *map_info, int x, int y)
{
	if (x < 0 || x >= map_info->width || y < 0 || y >= map_info->height)
		return (true);
	return (map_info->map[y][x] == '1');
}

void	update_game(t_data *data, t_player *player)
{
	const double	moveSpeed = 0.1;

	double	newX = player->posX + player->dirX * moveSpeed;
	double	newY = player->posY + player->dirY * moveSpeed;

	if (!check_wall(data->map, (int)newX, (int)player->posY))
	{
		player->posX = newX;
	}
	if (!check_wall(data->map, (int)player->posX, (int)newY))
	{
		player->posY = newY;
	}
}

int main(int ac, char **av)
{
    t_map    map_info = {0};
    t_data   data;
    t_mlx    mlx_info = {0};
    t_player player = {0};
    t_ray    ray = {0};
	t_texture texture = {0};

    if (check_arguments(ac, av) != 0)
        return (1);
    data.mlx = &mlx_info;
    data.map = &map_info;
    data.player = &player;
    if (parse_file(av[1], &map_info) != 0)
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
    if (!initialize_mlx_and_window(&mlx_info))
    {
        free_map(&map_info);
        return (1);
    }
    init_img(&data, mlx_info.mlx, MAX_WIDTH, MAX_HEIGHT);
    init_player(&player);
	init_texture(&mlx_info, &texture, av[1]);
    set_window(&data);
    mlx_hook(data.mlx->win, 2, 1L<<0, handle_input, &data);
    while (1)
    {
        update_game(&data, &player);
        for (int x = 0; x < MAX_WIDTH; x++)
        {
            cast_ray(&data, &player, x, &ray);
            int wall_height = (int)(MAX_HEIGHT / ray.perpWallDist);
            draw_wall_texture(&data, &ray, x, wall_height);
        }
        draw_map_2d(&data);
        mlx_put_image_to_window(data.mlx->mlx, data.mlx->win, data.mlx->img, 0, 0);
        mlx_loop(data.mlx->mlx);
    }
    free_map(&map_info);
    clean_exit(&mlx_info, &map_info);
    return (0);
}

