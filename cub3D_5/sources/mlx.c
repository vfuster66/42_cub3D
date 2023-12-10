/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 16:19:34 by vfuster-          #+#    #+#             */
/*   Updated: 2023/12/10 15:01:54 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

int	close_window(t_data *data)
{
	mlx_destroy_image(data->mlx->mlx, data->img->img);
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

void set_window(t_data *data)
{
    if (data == NULL || data->mlx == NULL || data->mlx->win == NULL || data->img == NULL || data->img->img == NULL) {
        fprintf(stderr, "Erreur de pointeur null dans set_window.\n");
        exit(EXIT_FAILURE);
    }

    mlx_key_hook(data->mlx->win, keyboard_actions, data);

    mlx_put_image_to_window(data->mlx->mlx, data->mlx->win, data->img->img, 0, 0);

    mlx_hook(data->mlx->win, 17, 0L, close_window, data);
}