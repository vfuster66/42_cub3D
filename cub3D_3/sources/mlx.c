/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 16:19:34 by vfuster-          #+#    #+#             */
/*   Updated: 2023/12/04 08:27:38 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"
#include "../includes/color.h"

/* MLX */
void	set_image(char **av, t_mlx *mlx)
{
	mlx->mlx = mlx_init();
	if (!(mlx->mlx))
	{
		perror("mlx_init error");
		exit(0);
	}
	mlx->title = ft_strjoin("cub3D: ", av[1]);
	mlx->win = mlx_new_window(mlx->mlx, MAX_WIDTH, MAX_HEIGHT, mlx->title);
	mlx->img = mlx_new_image(mlx->mlx,  MAX_WIDTH, MAX_HEIGHT);
	mlx->addr = mlx_get_data_addr(mlx->img, &mlx->bits_per_pixel,
			&mlx->line_length, &mlx->endian);
}

int	close_window(t_data *data)
{
	mlx_destroy_image(data->mlx->mlx, data->mlx->img);
	mlx_destroy_window(data->mlx->mlx, data->mlx->win);
	mlx_destroy_display(data->mlx->mlx);
	free(data->mlx->mlx);
	exit(0);
}

int	keyboard_actions(int key, t_data *data)
{
	if (key == ESC)
		close_window(data);
	mlx_do_sync(data->mlx->mlx);
	mlx_put_image_to_window(data->mlx->mlx, data->mlx->win,
		data->mlx->img, 0, 0);
	return (0);
}

void	set_window(t_data *data)
{
	mlx_key_hook(data->mlx->win, keyboard_actions, data);
	mlx_put_image_to_window(data->mlx->mlx, data->mlx->win,
		data->mlx->img, 0, 0);
	mlx_hook(data->mlx->win, 17, 0L, close_window, data);
	mlx_loop(data->mlx->mlx);
}
