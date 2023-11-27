/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorelli <lmorelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 20:23:26 by lmorelli          #+#    #+#             */
/*   Updated: 2023/09/11 19:08:08 by lmorelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_cd(char **args, int arg_nb, t_shell *shell)
{
	char	current_directory[PATH_MAX];

	(void)shell;
	if (arg_nb == 0)
	{
		perror("chdir");
		return ;
	}
	if (arg_nb > 1)
	{
		printf("cd: too many arguments\n");
		return ;
	}
	if (strcmp(args[0], "..") == 0) // Go back to the previous directory
	{
		if (chdir("..") == 0)
		{
			if (getcwd(current_directory, sizeof(current_directory)) != NULL)
				printf("Current directory: %s\n", current_directory);
			else
				perror("getcwd");
		}
		else
			perror("chdir");
		return ;
	}
	if (chdir(args[0]) == 0) // Change to the specified directory
	{
		if (getcwd(current_directory, sizeof(current_directory)) != NULL)
			printf("Current directory: %s\n", current_directory);
		else
			perror("getcwd");
	}
	else
		perror("chdir");
}
