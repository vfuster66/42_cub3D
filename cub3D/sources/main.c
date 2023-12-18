/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 09:02:58 by vfuster-          #+#    #+#             */
/*   Updated: 2023/12/14 16:08:02 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

static int	parse_args(t_data *data, char **av)
{
	if (check_file(av[1], true) == 1)
		exit_cleanup(data, 1);
	parse_data(av[1], data);
	if (get_file_data(data, data->mapinfo.file) == 1)
		return (free_data(data));
	if (check_map(data, data->map) == 1)
		return (free_data(data));
	if (check_textures_validity(data, &data->texinfo) == 1)
		return (free_data(data));
	init_player_direction(data);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 2)
		return (error_msg("Usage", ERROR_USAGE, 1));
	init_data(&data);
	if (parse_args(&data, av) != 0)
		return (1);
	init_mlx(&data);
	init_textures(&data);
	render_images(&data);
	listen_for_input(&data);
	mlx_loop_hook(data.mlx, render, &data);
	mlx_loop(data.mlx);
	return (0);
}
