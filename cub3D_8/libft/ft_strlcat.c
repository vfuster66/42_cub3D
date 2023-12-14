/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecullier <ecullier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 13:57:56 by ecullier          #+#    #+#             */
/*   Updated: 2023/12/14 13:57:59 by ecullier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
	DESCRIPTION :
	The function ft_strlcat appends the given string src to the end of 
	dst. It will append at most dstsize - ft_strlen(dst) - 1 and 
	nul-terminate the result.

	Note : space for the terminating \0 character must be included in dstsize.

	RETURN VALUE :
	The total length of the string that it tried to create : the initial
	length of dst + the length of src, with the goal to facilitate
	truncaction detection.
*/

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	d_size;
	size_t	s_size;

	d_size = ft_strlen(dst);
	s_size = ft_strlen(src);
	if (dstsize <= d_size)
		return (dstsize + s_size);
	i = d_size;
	j = 0;
	while ((i + j) < (dstsize - 1) && src[j] != '\0')
	{
		dst[i + j] = src[j];
		j++;
	}
	dst[i + j] = '\0';
	return (d_size + s_size);
}
