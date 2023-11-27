#include "minishell.h"

static int	ft_strtok_loop(char *next_token, const char *delim, t_shell *shell)
{
	int	match;

	match = 1;
	while (++shell->strtok.i < shell->strtok.delim_len)
	{
		if (*next_token == delim[shell->strtok.i])
		{
			match = 0;
			break ;
		}
	}
	return (match);
}

char	*ft_strtok(char *str, const char *delim, t_shell *shell)
{
	static char	*next_token = NULL;

	if (str != NULL)
		next_token = str;
	else if (next_token == NULL)
		return (NULL);
	shell->strtok.result = next_token;
	shell->strtok.delim_len = strlen(delim);
	while (*next_token != '\0')
	{
		shell->strtok.i = -1;
		shell->strtok.match = ft_strtok_loop(next_token, delim, shell);
		if (shell->strtok.match)
			next_token++;
		else
		{
			*next_token = '\0';
			next_token++;
			return (shell->strtok.result);
		}
	}
	next_token = NULL;
	return (shell->strtok.result);
}
