/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 12:16:10 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/02 09:17:05 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

int	variable(const char *name, const char *value)
{
	char		*env_var;
	size_t		len;

	len = strlen(name) + strlen(value) + 2;
	env_var = (char *)malloc(len);
	if (env_var == NULL)
	{
		errno = ENOMEM;
		return (-1);
	}
	strcpy(env_var, name);
	strcat(env_var, "=");
	strcat(env_var, value);
	if (ft_putenv(env_var) != 0)
	{
		free(env_var);
		errno = ENOMEM;
		return (-1);
	}
	return (0);
}

int	ft_setenv(const char *name, const char *value, int overwrite)
{
	if (name == NULL || name[0] == '\0' || strchr(name, '=') != NULL)
	{
		errno = EINVAL;
		return (-1);
	}
	if (getenv(name) != NULL && overwrite == 0)
	{
		return (0);
	}
	return (variable(name, value));
}
