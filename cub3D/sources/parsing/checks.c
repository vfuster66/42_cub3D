/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 16:18:51 by vfuster-          #+#    #+#             */
/*   Updated: 2023/12/14 15:59:58 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

static bool	is_directory(char *arg)
{
	int		fd;
	bool	ret;

	ret = false;
	fd = open(arg, O_DIRECTORY);
	if (fd >= 0)
	{
		close (fd);
		ret = true;
	}
	return (ret);
}

static bool	is_cub_file(char *arg)
{
	size_t	len;

	len = ft_strlen(arg);
	if ((arg[len - 3] != 'c' || arg[len - 2] != 'u'
			|| arg[len - 1] != 'b'
			|| arg[len - 4] != '.'))
		return (false);
	return (true);
}

static bool	is_xpm_file(char *arg)
{
	size_t	len;

	len = ft_strlen(arg);
	if ((arg[len - 3] != 'x' || arg[len - 2] != 'p'
			|| arg[len - 1] != 'm'
			|| arg[len - 4] != '.'))
		return (false);
	return (true);
}

int	check_file(char *arg, bool cub)
{
	int	fd;

	if (is_directory(arg))
		return (error_msg(arg, ERROR_FILE_IS_DIRECTORY, 1));
	fd = open(arg, O_RDONLY);
	if (fd == -1)
		return (error_msg(arg, strerror(errno), 1));
	close(fd);
	if (cub && !is_cub_file(arg))
		return (error_msg(arg, ERROR_FILE_NOT_CUB, 1));
	if (!cub && !is_xpm_file(arg))
		return (error_msg(arg, ERROR_FILE_NOT_XPM, 1));
	return (0);
}
