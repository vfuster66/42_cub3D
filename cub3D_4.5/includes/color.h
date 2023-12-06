/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 11:38:42 by ecullier          #+#    #+#             */
/*   Updated: 2023/12/04 08:37:22 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_H
# define COLOR_H

#include "cub3D.h"

typedef struct s_col_name
{
	char*	name;
	int		value;
}				t_col_name;

extern t_col_name mlx_col_name[];

#endif 