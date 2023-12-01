/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 09:02:58 by vfuster-          #+#    #+#             */
/*   Updated: 2023/12/01 12:04:26 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

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
