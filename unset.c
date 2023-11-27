/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorelli <lmorelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 20:25:25 by lmorelli          #+#    #+#             */
/*   Updated: 2023/09/11 19:20:36 by lmorelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
static int ft_unsetenv(const char *name)
{
	int i;
	int j;

	i = 0;
    if (name == NULL || name[0] == '\0' || strchr(name, '=') != NULL) {
        errno = EINVAL;
        return -1;
    }
    while (environ[i])
	{
        if (strncmp(environ[i], name, strlen(name)) == 0 && environ[i][strlen(name)] == '=')
		{
			j = i;
            while (environ[j])
			{
                environ[j] = environ[j + 1];
				j++;
            }
            return 0;
        }
		i++;
    }
    errno = ENOENT;
    return -1;
}*/


void	handle_unset(char **args, int arg_nb, t_shell *shell)
{
	char	*variable;

	(void)shell;
	variable = args[0];
	if (arg_nb != 1)
	{
		printf("Usage: unset VARIABLE\n");
		return ;
	}
	if (unsetenv(variable) == 0)   //da trovare alternativa a unsetenv
		printf("Unset: %s\n", variable);
	else
		perror("unsetenv");
}
