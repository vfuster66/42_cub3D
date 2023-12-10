/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 09:02:58 by vfuster-          #+#    #+#             */
/*   Updated: 2023/12/10 15:03:09 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

int initialize_mlx_and_window(t_mlx *mlx_info)
{
    mlx_info->mlx = mlx_init();

    if (mlx_info->mlx == NULL)
    {
        return (0);
    }
    mlx_info->win = mlx_new_window(mlx_info->mlx, MAX_WIDTH, MAX_HEIGHT, "Cub3D");
    if (mlx_info->win == NULL)
    {
        mlx_destroy_display(mlx_info->mlx);
        free(mlx_info->mlx);
        return (0);
    }
    return (1);
}

bool	check_wall(t_map *map_info, int x, int y)
{
	if (x < 0 || x >= map_info->width || y < 0 || y >= map_info->height)
		return (true);
	return (map_info->map[y][x] == '1');
}

void update_game(t_data *data, t_player *player)
{
    const double moveSpeed = 0.1;
    double buffer = 0.1;
    double newX = player->posX + player->dirX * moveSpeed;
    double newY = player->posY + player->dirY * moveSpeed;

    if (!check_wall(data->map, (int)(newX + buffer * (newX > player->posX ? 1 : -1)), (int)player->posY))
    {
        player->posX = newX;
    }

    if (!check_wall(data->map, (int)player->posX, (int)(newY + buffer * (newY > player->posY ? 1 : -1))))
    {
        player->posY = newY;
    }
}

int game_loop(t_data *data)
{
    update_game(data, data->player); 
    render(data);

    return 0;
}

int main(int ac, char **av)
{
    t_map    map_info = {0};
    t_data   data;
    t_mlx    mlx_info = {0};
    t_player player = {0};

    if (check_arguments(ac, av) != 0)
    {
        printf("Argument check failed.\n");
        return (1);
    }
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
        printf("Failed to initialize MLX and window\n");
        free_map(&map_info);
        return (1);
    }
    init_img(&data, mlx_info.mlx, MAX_WIDTH, MAX_HEIGHT);
    init_player(&player, &map_info);
    if (!init_all_textures(&mlx_info, &map_info))
    {
        printf("Failed to initialize textures\n");
        free_map(&map_info);
        clean_exit(&mlx_info, &map_info);
        return (1);
    }
    set_window(&data);
    mlx_loop_hook(data.mlx->mlx, game_loop, &data);
    mlx_key_hook(data.mlx->win, handle_input, &data);
    mlx_loop(data.mlx->mlx);
    free_map(&map_info);
    clean_exit(&mlx_info, &map_info);
    return (0);
}

