/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 09:02:58 by vfuster-          #+#    #+#             */
/*   Updated: 2023/12/10 21:22:53 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

//t_key_state g_keystate;

int init_mlx_and_window(t_mlx *mlx_info)
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
    const double move_speed = 0.1;
    double buffer = 0.1;
    double new_x = player->pos_x + player->dir_x * move_speed;
    double new_y = player->pos_y + player->dir_y * move_speed;

    if (!check_wall(data->map, (int)(new_x + buffer * (new_x > player->pos_x ? 1 : -1)), (int)player->pos_y))
    {
        player->pos_x = new_x;
    }

    if (!check_wall(data->map, (int)player->pos_x, (int)(new_y + buffer * (new_y > player->pos_y ? 1 : -1))))
    {
        player->pos_y = new_y;
    }
}

int game_loop(t_data *data)
{
    update_game(data, data->player); 
    render(data);
    return 0;
}

int initialize_game(char *filename, t_data *data, t_mlx *mlx_info, t_map *map_info, t_player *player)
{
    if (parse_file(filename, map_info) != 0)
    {
        printf("Error parsing the map file.\n");
        return (1);
    }
    if (validate_map(map_info) != 0)
    {
        printf("Invalid map.\n");
        free_map(map_info);
        return (1);
    }
    if (!init_mlx_and_window(mlx_info))
    {
        printf("Failed to initialize MLX and window\n");
        free_map(map_info);
        return (1);
    }
    init_img(data, mlx_info->mlx, MAX_WIDTH, MAX_HEIGHT);
    init_player(player, map_info);
    if (!init_all_textures(mlx_info, map_info))
    {
        printf("Failed to initialize textures\n");
        free_map(map_info);
        clean_exit(mlx_info, map_info);
        return (1);
    }
    set_window(data);
    return (0);
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
    
    t_key_state key_state;
    memset(&key_state, 0, sizeof(t_key_state));

    
    if (initialize_game(av[1], &data, &mlx_info, &map_info, &player) != 0)
    {
        return (1);
    }
    mlx_loop_hook(data.mlx->mlx, game_loop, &data);
    mlx_key_hook(data.mlx->win, handle_input, &data);
    mlx_loop(data.mlx->mlx);
    free_map(&map_info);
    clean_exit(&mlx_info, &map_info);
    return (0);
}
