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

int	initialize_mlx_and_window(t_mlx *mlx_info)
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

void	cleanup(t_map *map_info, t_mlx *mlx_info)
{
	free_map(map_info);
	mlx_destroy_window(mlx_info->mlx, mlx_info->win);
	mlx_destroy_display(mlx_info->mlx);
	free(mlx_info->mlx);
}

int	main(int ac, char **av)
{
	int	parse_status;

	t_map	map_info = {0};
	t_data	data;
	t_mlx	mlx_info;

	if (check_arguments(ac, av) != 0)
		return (1);
	if (!initialize_mlx_and_window(&mlx_info))
		return (1);
	parse_status = parse_file(av[1], &map_info);
	if (parse_status == 0)
	{
		printf("Error parsing the map file.\n");
	}
	else
	{
		data.mlx = &mlx_info;
		set_image(av, &mlx_info);
		set_window(&data);
	}
	cleanup(&map_info, &mlx_info);
	return (parse_status != 0);
}
