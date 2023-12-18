/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecullier <ecullier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 10:15:43 by ecullier          #+#    #+#             */
/*   Updated: 2023/12/14 10:16:21 by ecullier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3D.h"

static int	check_rgb(int *rgb)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		if (rgb[i] < 0 || rgb[i] > 255)
			return (error_msg_val(rgb[i], ERROR_TEX_RGB_VAL, 1));
		i++;
	}
	return (0);
}

static unsigned long	convert_rgb_to_hex(int *rgb_tab)
{
	unsigned long	result;
	int				r;
	int				g;
	int				b;

	r = rgb_tab[0];
	g = rgb_tab[1];
	b = rgb_tab[2];
	result = ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
	return (result);
}

int	check_textures_validity(t_data *data, t_texture *textures)
{
	if (!textures->north || !textures->south || !textures->west
		|| !textures->east)
		return (error_msg(data->mapinfo.path, ERROR_TEX_MISSING, 1));
	if (!textures->floor || !textures->ceiling)
		return (error_msg(data->mapinfo.path, ERROR_COLOR_MISSING, 1));
	if (check_file(textures->north, false) == 1
		|| check_file(textures->south, false) == 1
		|| check_file(textures->west, false) == 1
		|| check_file(textures->east, false) == 1
		|| check_rgb(textures->floor) == 1
		|| check_rgb(textures->ceiling) == 1)
		return (1);
	textures->hex_floor = convert_rgb_to_hex(textures->floor);
	textures->hex_ceiling = convert_rgb_to_hex(textures->ceiling);
	return (0);
}
