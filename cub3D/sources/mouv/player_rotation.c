/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_rotation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 10:10:22 by ecullier          #+#    #+#             */
/*   Updated: 2023/12/14 16:01:16 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3D.h"

static int	rotate_left_right(t_data *data, double rot_speed)
{
	t_player	*p;
	double		tmp_x;

	p = &data->player;
	tmp_x = p->dir_x;
	p->dir_x = p->dir_x * cos(rot_speed) - p->dir_y * sin(rot_speed);
	p->dir_y = tmp_x * sin(rot_speed) + p->dir_y * cos(rot_speed);
	tmp_x = p->plane_x;
	p->plane_x = p->plane_x * cos(rot_speed) - p->plane_y * sin(rot_speed);
	p->plane_y = tmp_x * sin(rot_speed) + p->plane_y * cos(rot_speed);
	return (1);
}

int	rotate_player(t_data *data, double rot_dir)
{
	int		moved;
	double	rot_speed;

	moved = 0;
	rot_speed = ROTSPEED * rot_dir;
	moved += rotate_left_right(data, rot_speed);
	return (moved);
}
