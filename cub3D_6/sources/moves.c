/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 22:06:42 by parallels         #+#    #+#             */
/*   Updated: 2023/12/10 21:40:09 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

t_key_state g_keystate = {0};

void	move_forward(t_player *player, double move_speed)
{
	player->pos_x += player->dir_x * move_speed;
	player->pos_y += player->dir_y * move_speed;
}

void	move_backward(t_player *player, double move_speed)
{
	player->pos_x -= player->dir_x * move_speed;
	player->pos_y -= player->dir_y * move_speed;
}

void rotate_right(t_player *player, double rot_speed)
{
	double olddir_x;
	double old_plane_x;

	olddir_x = player->dir_x;
	player->dir_x = player->dir_x * cos(-rot_speed) - player->dir_y * sin(-rot_speed);
	player->dir_y = olddir_x * sin(-rot_speed) + player->dir_y * cos(-rot_speed);
	old_plane_x = player->plane_x;
	player->plane_x = player->plane_x * cos(-rot_speed) - player->plane_y * sin(-rot_speed);
	player->plane_y = old_plane_x * sin(-rot_speed) + player->plane_y * cos(-rot_speed);
}

void	rotate_left(t_player *player, double rot_speed)
{
	double olddir_x;
	double old_plane_x;

	olddir_x = player->dir_x;
	player->dir_x = player->dir_x * cos(rot_speed) - player->dir_y * sin(rot_speed);
	player->dir_y = olddir_x * sin(rot_speed) + player->dir_y * cos(rot_speed);
	old_plane_x = player->plane_x;
	player->plane_x = player->plane_x * cos(rot_speed) - player->plane_y * sin(rot_speed);
	player->plane_y = old_plane_x * sin(rot_speed) + player->plane_y * cos(rot_speed);
}

void	move_left(t_player *player, double move_speed)
{
	player->pos_x -= player->plane_x * move_speed;
	player->pos_y -= player->plane_y * move_speed;
}

void	move_right(t_player *player, double move_speed)
{
	player->pos_x += player->plane_x * move_speed;
	player->pos_y += player->plane_y * move_speed;
}

/*int	handle_input(int keycode, t_data *data)
{
	const double	move_speed = 0.1;
	const double	rot_speed = 0.1;

	if (keycode == W_KEY)
		move_forward(data->player, move_speed);
	if (keycode == S_KEY)
		move_backward(data->player, move_speed);
	if (keycode == A_KEY)
		move_left(data->player, move_speed);
	if (keycode == D_KEY)
		move_right(data->player, move_speed);
	if (keycode == RIGHT_KEY)
		rotate_right(data->player, rot_speed);
	if (keycode == LEFT_KEY)
		rotate_left(data->player, rot_speed);
	return (0);
}*/

int handle_input(int keycode, t_data *data) {
    const double move_speed = 0.3;
    const double rot_speed = 0.1;

    if (keycode == W_KEY)
		g_keystate.W = 1;
    if (keycode == S_KEY)
		g_keystate.S = 1;
    if (keycode == A_KEY)
		g_keystate.A = 1;
    if (keycode == D_KEY)
		g_keystate.D = 1;

    if (g_keystate.W) move_forward(data->player, move_speed);
    if (g_keystate.S) move_backward(data->player, move_speed);
    if (g_keystate.A) move_left(data->player, move_speed);
    if (g_keystate.D) move_right(data->player, move_speed);
    if (keycode == RIGHT_KEY) rotate_right(data->player, rot_speed);
    if (keycode == LEFT_KEY) rotate_left(data->player, rot_speed);

    return (0);
}


