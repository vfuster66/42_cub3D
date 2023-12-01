/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 11:33:00 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/02 07:51:02 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

extern char	**environ;

static int	replace_env_var(char *name, char *value)
{
	char	**env_ptr;

	env_ptr = environ;
	while (*env_ptr != NULL)
	{
		if (ft_strncmp(name, *env_ptr, strlen(name)) == 0
			&& (*env_ptr)[strlen(name)] == '=')
		{
			free(*env_ptr);
			*env_ptr = ft_strdup(name);
			ft_strcat(*env_ptr, "=");
			ft_strcat(*env_ptr, value);
			return (0);
		}
		env_ptr++;
	}
	return (-1);
}

static int	add_env_var(char *string)
{
	int	env_count;

	env_count = 0;
	while (environ[env_count] != NULL)
	{
		env_count++;
	}
	environ[env_count] = ft_strdup(string);
	environ[env_count + 1] = NULL;
	return (0);
}

int	ft_putenv(char *string)
{
	char	*name;
	char	*value;

	if (string == NULL || ft_strchr(string, '=') == NULL)
	{
		return (-1);
	}
	name = ft_strtok(string, "=");
	value = ft_strtok(NULL, "=");
	if (name == NULL || value == NULL)
	{
		return (-1);
	}
	if (replace_env_var(name, value) == 0)
	{
		return (0);
	}
	else
	{
		return (add_env_var(string));
	}
}
