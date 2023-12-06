/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmorelli <lmorelli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 21:13:28 by lmorelli          #+#    #+#             */
/*   Updated: 2023/09/11 19:26:08 by lmorelli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include <dirent.h>
# include <sys/types.h>
# include <errno.h>
# include <sys/wait.h>

# define MAX_ARGS 10

typedef struct s_strtok
{
	size_t	delim_len;
	char	*result;
	int		match;
	size_t	i;
}	t_strtok;

typedef struct s_shell{
	char		*input;
	char		**command;
	int			stdout;
	int			stdin;
	t_strtok	strtok;
}	t_shell;

void	handle_echo(char **args, int arg_nb, t_shell *shell);
void	handle_pwd(char **args, int arg_nb, t_shell *shell);
void	handle_exit(char **args, int arg_nb, t_shell *shell);
void	handle_cd(char **args, int arg_nb, t_shell *shell);
void	handle_export(char **args, int arg_nb, t_shell *shell);
void	handle_unset(char **args, int arg_nb, t_shell *shell);
void	handle_env(char **args, int arg_nb, t_shell *shell);

char	*ft_strtok(char *str, const char *delim, t_shell *shell);

struct CommandFunction {
	char	*commandName;
	void	(*functionPtr)(char **args, int argCount, t_shell *shell);
};

// Dichiarazione esterna di commandFunctions
extern struct CommandFunction	commandFunctions[];
//Se hai una variabile globale definita in un file
//sorgente e vuoi utilizzarla in un altro file sorgente, devi dichiararla come extern in questo secondo file.

int execute_external_command(char **args);

#endif