/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorelli <lmorelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 21:11:36 by lmorelli          #+#    #+#             */
/*   Updated: 2023/09/11 19:29:15 by lmorelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_exit(char **args, int arg_nb, t_shell *shell)
{
	(void)args;
	(void)arg_nb;
	(void)shell;
	exit (0);
}

static void	start_with_readline(t_shell *shell)
{
	char	*args[MAX_ARGS];
	int		arg_nb;
	int		i;
	struct CommandFunction	commandFunctions[] = {
	{"echo", handle_echo},
	{"pwd", handle_pwd},
	{"exit", handle_exit},
	{"cd", handle_cd},
	{"export", handle_export},
	{"unset", handle_unset},
	{"env", handle_env},
	};
	int is_builtin;

	
	while (1)
	{
		is_builtin = 0;
		rl_bind_key('\t', rl_complete);
		//Viene inizializzata la libreria readline.
		//La funzione rl_bind_key imposta la gestione del tasto Tab per il completamento automatico durante la digitazione.
		shell->input = readline("minishell: "); //readline è una libreria che semplifica la lettura
		//dell'input da parte dell'utente in una modalità interattiva. In questo caso
		//viene utilizzata per leggere una riga di testo dall'utente. L'argomento "minishell: " è il prompt che verrà mostrato all'utente.
		if (shell->input && *shell->input) // Aggiungi la shell->input alla cronologia dei comandi (add_history fa parte di readline)
			add_history(shell->input);
		shell->command = ft_strtok(shell->input, " ", shell);    //ft_strtok è una funzione che estrae parti di una stringa
		//(in questo caso, l'input dell'utente) in base a un delimitatore specificato (in questo caso, lo spazio " ").
		//La prima chiamata a strtok estrae il primo "token" (in questo caso, il comando) dalla stringa.
		arg_nb = 0; 
		// Questo ciclo while estrae gli argomenti dal resto dell'input.
		//Continua a chiamare ft_strtok fino a quando ha estratto tutti gli argomenti o fino a quando raggiunge il limite
		//massimo di argomenti (MAX_ARGS - 1). Ogni chiamata successiva a ft_strtok estrae il token successivo dalla stringa.
		while (arg_nb < MAX_ARGS - 1)
		{
			args[arg_nb] = ft_strtok(NULL, " ", shell);
			if (args[arg_nb] == NULL)
				break ;
			arg_nb++;
		}
		args[arg_nb] = NULL;
/*
    Stampa il comando e gli argomenti
          printf("Comando: %s\n", shell->command);
          printf("Argomenti:\n");
          while (++i < arg_nb) 
              printf("Arg %d: %s\n", i + 1, args[i]);
*/
		if (strcmp(shell->command, "cd") == 0)
			handle_cd(args, arg_nb, shell);
		/*
		Il comando "cd" è un comando interno al sistema operativo che cambia la directory corrente del processo chiamante.
		Questo comando non viene eseguito in un processo separato come gli altri comandi. Cambiare la directory in un processo
		separato non avrebbe alcun effetto sulla directory corrente del processo genitore (la shell). Quindi, è necessario gestire
		il comando "cd" direttamente nella shell principale.
		*/

		else
		{
			i = -1;
			//Se il comando inserito dall'utente non è "cd", allora il programma cerca una corrispondenza nel vettore commandFunctions.
			//Questo vettore contiene nomi di comandi e puntatori alle funzioni associate a ciascun comando. Se viene trovata una
			//corrispondenza, la funzione associata a quel comando verrà chiamata per eseguire l'azione corrispondente.
			while ((long unsigned int)++i < sizeof(commandFunctions) / sizeof(struct CommandFunction)) 
			{
				if (strcmp(commandFunctions[i].commandName, shell->command) == 0) 
				{
					commandFunctions[i].functionPtr(args, arg_nb, shell);
					is_builtin = 1;
					break ;
				}
			}
		}
		if (!is_builtin)
		{
			char *ext_args[] = {shell->command, 0, NULL};
			execute_external_command(ext_args, shell);
			is_builtin = 0;
			wait(NULL); //Attendo che il processo figlio completi l'esecuzione prima di tornare al prompt
		}
	}
//free(shell->input);
}

int	main(int argc, char **argv)
{
	t_shell	shell;

	(void)argv;
	if (argc > 1)
		exit(printf("Error, there are too many arguments!!\n"));
	shell.stdout = dup(STDOUT_FILENO);
/*
    Questa riga sta duplicando il file descriptor standard di output 
    (STDOUT_FILENO) nella variabile shell.stdout della struttura shell. In altre parole,
    stai creando una copia del file descriptor di output standard. Utile per i reindirizzamenti.
*/
	shell.stdin = dup(STDIN_FILENO);
	start_with_readline(&shell);
    //char *args[] = { shell.command, NULL };
	return (0);
}
