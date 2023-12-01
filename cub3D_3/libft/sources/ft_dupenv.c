/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dupenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 12:56:43 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/20 11:35:43 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"
// #include <stdio.h>

char	**initialize_environment_copy(char **environ)
{
	int		env_size;
	char	**env_copy;
	int		i;

	env_size = 0;
	while (environ[env_size] != NULL)
	{
		env_size++;
	}
	env_copy = (char **)malloc((env_size + 1) * sizeof(char *));
	i = 0;
	while (i <= env_size)
	{
		env_copy[i] = NULL;
		i++;
	}
	return (env_copy);
}

char	*copy_environment_variable(char *env_var)
{
	char	*copy;

	copy = ft_strdup(env_var);
	if (copy == NULL)
	{
		return (NULL);
	}
	return (copy);
}

char	**ft_dupenv(char **environ)
{
	char	**env_copy;
	int		i;

	i = 0;
	env_copy = initialize_environment_copy(environ);
	while (environ[i] != NULL)
	{
		env_copy[i] = copy_environment_variable(environ[i]);
		i++;
	}
	return (env_copy);
}
// int main(int ac, char **av, char **env)
// {
// 	(void)ac;
// 	(void)av;
// 	char **copy_env;
// 	copy_env = ft_dupenv(env);
// 	// return(0);
// 	int	i;

// 	i = 0;
// 	while (copy_env[i] != NULL)
// 		i++;
// 	i--;
// 	printf("i = %d\n", i);
// 	if (copy_env != NULL)
// 	{
// 		// printf("copy_env[59] : %s\n", copy_env[59]);
// 		while (i != -1)
// 		{
// 			printf("%d\n", i);
// 			free(copy_env[i]);
// 			i--;
// 		}
// 		// printf("copy_env[%d] : %s\n", i, copy_env[i]);
// 		free(copy_env);
// 	}
// 	return(0);
// }
