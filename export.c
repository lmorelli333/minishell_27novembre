/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorelli <lmorelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 20:24:52 by lmorelli          #+#    #+#             */
/*   Updated: 2023/09/11 19:28:11 by lmorelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_export(char **args, int arg_nb, t_shell *shell)
{
	char	*arg;
	char	*variable;
	char	*value;

	(void)shell;
	arg = args[0];
	variable = ft_strtok(arg, "=", shell);
	value = ft_strtok(NULL, "=", shell);
	if (arg_nb != 1)
	{
		printf("Usage: export VARIABLE=VALUE\n");
		return ;
	}
	if (variable == NULL || value == NULL)
	{
		printf("Invalid format. Usage: export VARIABLE=VALUE\n");
		return ;
	}
	if (setenv(variable, value, 1) == 0) //da trovare alternativa a setenv
		printf("Exported: %s=%s\n", variable, value);
	else
		perror("setenv");
}
