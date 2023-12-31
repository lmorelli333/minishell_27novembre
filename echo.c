/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorelli <lmorelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 20:19:50 by lmorelli          #+#    #+#             */
/*   Updated: 2023/09/11 19:06:29 by lmorelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_echo(char **args, int arg_nb, t_shell *shell)
{
	int		output_redirect_index = -1;
	int		input_redirect_index = 1;
	int		output_redirect = -1;
    // int input_redirect = -1;
	int		i;
	char	*echo_args[MAX_ARGS];
	int		original_stdout;
	int		no_newline = 0; // Initialize to 0, meaning newline will be added by default

	(void)shell;
	i = 0;
	if (strcmp(args[0], "-n") == 0)
		no_newline = 1; 
    // Trova l'indice del reindirizzamento dell'output (>)
	while (i < arg_nb) 
	{
		if (strcmp(args[i], ">") == 0) 
		{
			output_redirect_index = i;
			break ;
		}
		else if (strcmp(args[i], "<") == 0)
		{
			input_redirect_index = 0;
			break ;
		}
		else if (strcmp(args[i], ">>") == 0)
		{
			output_redirect = i;
			break ;
		}
		i++;
	}
// Creare un processo figlio
	pid_t	child_pid = fork();

	if (child_pid == -1)
	{
		perror("fork");
		return ;
	}
	if (child_pid == 0)
	{
		if (output_redirect_index != -1)
		{
        // Apri il file specificato dopo il carattere '>'
			FILE *output_file = fopen(args[output_redirect_index + 1], "w");
			if (output_file != NULL)
			{
            // Crea una copia dei comandi e degli argomenti del comando "echo"
				i = 0;
				while (i < output_redirect_index)
				{
					echo_args[i] = args[i];
					i++;
				}
				echo_args[output_redirect_index] = NULL;
                // Reindirizza l'output verso il file
				original_stdout = dup(STDOUT_FILENO);
				dup2(fileno(output_file), STDOUT_FILENO);
				fclose(output_file);
                // Stampa il comando "echo" nel file
				if (no_newline)
					i = 1;
				else
					i = 0;
				while (echo_args[i] != NULL)
				{
					write(STDOUT_FILENO, echo_args[i], strlen(echo_args[i]));
					write(STDOUT_FILENO, " ", 1);
					i++;
				}
				if (!no_newline)
					write(STDOUT_FILENO, "\n", 1);
            // Ripristina il reindirizzamento dell'output standard
				dup2(original_stdout, STDOUT_FILENO);
				close(original_stdout);
			}
		}
		if (output_redirect != -1)
		{
			if (args[output_redirect + 1] == NULL)
			{
				printf("env: Missing filename after '>>'\n");
				exit(1);
			}
            // Apri il file in modalità append
			FILE *output_file = fopen(args[output_redirect + 1], "a");
			if (output_file == NULL)
			{
				perror("fopen");
				exit(1);
			}
            // Rimuovi gli argomenti di reindirizzamento dal comando echo
			args[output_redirect] = NULL;
			args[output_redirect + 1] = NULL;
            // Esegui l'env
			if (no_newline)
				i = 1;
			else
				i = 0;
			while (args[i] != NULL)
			{
				fprintf(output_file, "%s", args[i]);
				i++;
			}
			if (!no_newline)
				fprintf(output_file, "\n");
			fclose(output_file);
		}
		else
		{
            // Nessun reindirizzamento dell'output, stampa normalmente sulla console
			if (no_newline)
				i = 1;
			else
				i = 0;
			while (args[i] != NULL && input_redirect_index)
			{
				printf("%s ", args[i]);
				i++;
			}
			if (!no_newline) // Add newline unless -n option is present
				printf("\n");
		}
	}
	else
	{
        // Questo è il processo padre
        // Aspetta che il processo figlio termini
		int	status;
		waitpid(child_pid, &status, 0);
	}
}
