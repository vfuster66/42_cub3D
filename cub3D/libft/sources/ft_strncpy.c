/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 09:13:33 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/20 10:06:52 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	char	*dest_start;

	dest_start = dest;
	while (n > 0)
	{
		if (*src != '\0')
		{
			*dest = *src;
			dest++;
			src++;
		}
		else
			*dest = '\0';
		n--;
	}
	return (dest_start);
}
