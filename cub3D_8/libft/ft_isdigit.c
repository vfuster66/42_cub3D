/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecullier <ecullier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 13:53:22 by ecullier          #+#    #+#             */
/*   Updated: 2023/12/14 13:54:54 by ecullier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*
	DESCRIPTION :
	The function ft_isdigit checks whether c is a digit character or not.

	RETURN VALUE:
	Non-zero if c is a digit, zero if not.
*/

int	ft_isdigit(int c)
{
	if (c > 47 && c < 58)
		return (1);
	else
		return (0);
}
