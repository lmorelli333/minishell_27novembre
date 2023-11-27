/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorelli <lmorelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 20:24:12 by lmorelli          #+#    #+#             */
/*   Updated: 2023/09/11 19:18:52 by lmorelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pwd(char **args, int arg_nb, t_shell *shell)
{
	char	current_directory[PATH_MAX];

	(void)args;
	(void)shell;
	if (arg_nb != 0)
	{
		printf("Too much arguments\n");
		return ;
	}
	if (getcwd(current_directory, sizeof(current_directory)) != NULL)
		printf("%s\n", current_directory);
	else 
		perror("pwd");
}
