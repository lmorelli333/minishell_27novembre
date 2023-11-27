/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorelli <lmorelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 20:26:45 by lmorelli          #+#    #+#             */
/*   Updated: 2023/09/11 19:39:51 by lmorelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		ft_in_red_idx02(char **args, int in_red_idx01)
{
	const char	*env_cmd[2];

	env_cmd[0] = "env";
	env_cmd[1] = NULL;
	if (args[in_red_idx01 + 2] == NULL)
	{
		printf("env: Missing here document after '<'\n");
		exit(1);
	}
	execvp("env", (char *const *)env_cmd); //da trovare alternativa a execvp
	perror("execvp");
	exit(1);
}

static void	ft_in_red_idx01(char **args, int in_red_idx01)
{
	const char		*env_cmd[2];

	env_cmd[0] = "env";
	env_cmd[1] = NULL;
	if (args[in_red_idx01 + 1] == NULL)
	{
		printf("env: Missing here document after '<<'\n");
		exit(1);
	}
	execvp("env", (char *const *)env_cmd);
	perror("execvp");
	exit(1);
}

static void	ft_out_red_idx02(char **args, int out_red_idx01)
{
	int				saved_stdout = dup(STDOUT_FILENO);
	const char			*env_cmd[2];

	env_cmd[0] = "env";
	env_cmd[1] = NULL;
	if (args[out_red_idx01 + 2] == NULL)
	{
		printf("env: Missing filename after '>'\n");
		exit(1);
	}
	FILE *output_file = fopen(args[out_red_idx01 + 2], "w");
	if (output_file == NULL)
	{
		perror("fopen");
		exit(1);
	}
    // Reindirizza l'output verso il file
	dup2(fileno(output_file), STDOUT_FILENO);
	fclose(output_file);
    // Esegui l'env
	execvp("env", (char *const *)env_cmd);
    // Ripristina l'output standard
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
	perror("execvp");
	exit(1);
}

static void	ft_out_red_idx01(char **args, int out_red_idx01)
{
	int			saved_stdout = dup(STDOUT_FILENO);
	const char	*env_cmd[2];

	env_cmd[0] = "env";
	env_cmd[1] = NULL;
	if (args[out_red_idx01 + 1] == NULL)
	{
		printf("env: Missing filename after '>>'\n");
		exit(1);
	}
    // Apri il file in modalità append
	FILE *output_file = fopen(args[out_red_idx01 + 1], "a");
	if (output_file == NULL)
	{
		perror("fopen");
		exit(1);
	}
    // Reindirizza l'output verso il file
	dup2(fileno(output_file), STDOUT_FILENO);
	fclose(output_file);
    // Esegui l'env
	execvp("env", (char *const *)env_cmd);
    // Ripristina l'output standard
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
	perror("execvp");
	exit(1);
}

static void	handle_env02(char **args, int out_red_idx01, int in_red_idx01, int out_red_idx02, int in_red_idx02)
{
	pid_t	child_pid = fork();// Creare un processo figlio
	int	status;
	const char *env_cmd[2];

	env_cmd[0] = "env";
	env_cmd[1] = NULL;
	if (child_pid == -1)
	{
		perror("fork");
		return ;
	}
	if (child_pid == 0)
	{
        // Questo è il processo figlio
        // Gestisci il redirect di output
		if (out_red_idx01 != -1)
			ft_out_red_idx01(args, out_red_idx01);
		if (out_red_idx02 != -1)
			ft_out_red_idx02(args, out_red_idx01);
        // Gestisci il redirect di input
		if (in_red_idx01 != -1)
			ft_in_red_idx01(args, in_red_idx01);
		if (in_red_idx02 != -1)
			ft_in_red_idx02(args, in_red_idx01);
        // Se non ci sono redirect, esegui semplicemente env
		execvp("env", (char *const *)env_cmd);
		perror("execvp");
		exit(1);
	}
	else
		waitpid(child_pid, &status, 0);// Questo è il processo padre e aspetta che il processo figlio termini
}

void	handle_env(char **args, int arg_nb, t_shell *shell)
{
	int	out_red_idx01;
	int	in_red_idx01;
	int	out_red_idx02;
	int	in_red_idx02;
	int	i;

	(void)arg_nb;
	(void)shell;
	out_red_idx01 = -1;
	in_red_idx01 = -1;
	out_red_idx02 = -1;
	in_red_idx02 = -1;
	i = -1;
	while (args[++i])
	{
		if (strcmp(args[i], ">>") == 0)
			out_red_idx01 = i;
		else if (strcmp(args[i], "<<") == 0) 
			in_red_idx01 = i;
		else if (strcmp(args[i], ">") == 0) 
			out_red_idx02 = i;
		else if (strcmp(args[i], "<") == 0) 
			in_red_idx02 = i;
	}
	handle_env02(args, out_red_idx01, in_red_idx01,
	out_red_idx02, in_red_idx02);
}
