/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 22:06:42 by parallels         #+#    #+#             */
/*   Updated: 2023/12/10 14:14:11 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

void	move_forward(t_player *player, double move_speed)
{
	player->posX += player->dirX * move_speed;
	player->posY += player->dirY * move_speed;
}

void	move_backward(t_player *player, double move_speed)
{
	player->posX -= player->dirX * move_speed;
	player->posY -= player->dirY * move_speed;
}

void rotate_right(t_player *player, double rot_speed)
{
	double oldDirX;
	double oldPlaneX;

	oldDirX = player->dirX;
	player->dirX = player->dirX * cos(-rot_speed) - player->dirY * sin(-rot_speed);
	player->dirY = oldDirX * sin(-rot_speed) + player->dirY * cos(-rot_speed);
	oldPlaneX = player->planeX;
	player->planeX = player->planeX * cos(-rot_speed) - player->planeY * sin(-rot_speed);
	player->planeY = oldPlaneX * sin(-rot_speed) + player->planeY * cos(-rot_speed);
}

void	rotate_left(t_player *player, double rot_speed)
{
	double oldDirX;
	double oldPlaneX;

	oldDirX = player->dirX;
	player->dirX = player->dirX * cos(rot_speed) - player->dirY * sin(rot_speed);
	player->dirY = oldDirX * sin(rot_speed) + player->dirY * cos(rot_speed);
	oldPlaneX = player->planeX;
	player->planeX = player->planeX * cos(rot_speed) - player->planeY * sin(rot_speed);
	player->planeY = oldPlaneX * sin(rot_speed) + player->planeY * cos(rot_speed);
}

void	move_left(t_player *player, double move_speed)
{
	player->posX -= player->planeX * move_speed;
	player->posY -= player->planeY * move_speed;
}

void	move_right(t_player *player, double move_speed)
{
	player->posX += player->planeX * move_speed;
	player->posY += player->planeY * move_speed;
}

int	handle_input(int keycode, t_data *data)
{
	const double	move_speed = 0.1;
	const double	rot_speed = 0.1;

	if (keycode == 'W') move_forward(data->player, move_speed);
	if (keycode == 'S') move_backward(data->player, move_speed);
	if (keycode == 'A') move_left(data->player, move_speed);
	if (keycode == 'D') move_right(data->player, move_speed);
	if (keycode == RIGHT) rotate_right(data->player, rot_speed);
	if (keycode == LEFT) rotate_left(data->player, rot_speed);
	return (0);
}
