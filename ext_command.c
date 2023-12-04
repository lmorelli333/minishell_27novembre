#include "minishell.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	size_t	index;

	if (!s)
		return (NULL);
	if (start >= strlen(s))
		start = strlen(s);
	if ((start + len) >= strlen(s))
		len = strlen(s) - start;
	ptr = malloc(sizeof(char) * (len + 1));
	if (!ptr)
		return (NULL);
	index = 0;
	while (index != len)
	{
		ptr[index] = *(s + start + index);
		index++;
	}
	ptr[index] = '\0';
	return (ptr);
}

static size_t	get_word(const char *s, char c)
{
	size_t	words;

	words = 0;
	while (*s)
	{
		if (*s != c)
		{
			++words;
			while (*s && *s != c)
				++s;
		}
		else
			++s;
	}
	return (words);
}

static char	**ft_split(const char *s, char c)
{
	char	**str;
	size_t	i;
	size_t	len;

	if (!s)
		return (0);
	i = 0;
	str = malloc(sizeof(char *) * (get_word(s, c) + 1));
	if (!str)
		return (0);
	while (*s)
	{
		if (*s != c)
		{
			len = 0;
			while (*s && *s != c && ++len)
				++s;
			str[i++] = ft_substr(s - len, 0, len);
		}
		else
			++s;
	}
	str[i] = 0;
	return (str);
}

int execute_external_command(char **args, t_shell *shell)
{
  pid_t pid;
  int status;
  char *path_env;
  //char full_path[256]; // dimensione massima path, va bene?
  char *env[] = {NULL}; //se non metto NULL non funziona. Serviranno valori diversi? Riguarda le variabili d ambiente
  char **token; 
  int i = 0;
  (void)shell;
  char *res;
  path_env = getenv("PATH");
  if (path_env == NULL)
  {
    perror("getenv error");
    return (0);
  }
  token = ft_split(path_env, ':');
  //token = ft_strtok(path_env, ":", shell);
  //printf("start token = %s\n", token);
  while (token[i] != NULL)
  {
   // strcpy(full_path, token);      // Copia il percorso nel buffer full_path
    res = strcat(token[i+1], "/");        // Aggiungi una barra (slash)
    res = strcat(token[i+1], args[0]);    // Concatena il comando alla fine del percorso
    //printf("full_path = %s\n", full_path);
    //printf("token = %s\n", token);
    if (access(res, X_OK) == 0)
    {
      pid = fork(); //viene creato un processo figlio
      if (pid == 0) //processo figlio
      {
        if (execve(res, args, env) == -1) // esegue il comando nel processo figlio
          perror("execve error");
        exit(EXIT_FAILURE);
      }
      else if (pid < 0) //errore nel fork
        perror("fork error");
      else //processo padre
      {
        while (!WIFEXITED(status) && !WIFSIGNALED(status)) // Il ciclo continua finché il processo figlio non termina con successo (WIFEXITED) o viene interrotto da un segnale (WIFSIGNALED).
          waitpid(pid, &status, 0);
      }
      return (1);
    }
    i++;
    //token = ft_strtok(NULL, ":", shell);
  }
  //perror ("error, the command doesn't exist or you don't have the necessary permissions");
  return (0);
}

/*

WIFEXITED e WIFSIGNALED sono macro definite nella libreria <sys/wait.h>
WIFEXITED(status): Restituisce un valore diverso da zero se il processo figlio è terminato normalmente.
WIFSIGNALED(status): Restituisce un valore diverso da zero se il processo figlio è stato terminato da un segnale.

Come funziona fork:

-Duplicazione del processo: Quando chiami fork, il sistema operativo crea una copia esatta del processo chiamato "processo figlio".
 Entrambi i processi (il processo padre e il processo figlio) iniziano a eseguire l'istruzione successiva a fork.

-Valore restituito dal fork: Nel processo padre, fork restituisce il PID (Process ID) del processo figlio appena creato. Nel processo figlio, fork restituisce 0.

-Duplicazione dei file descriptor: Entrambi i processi (padre e figlio) condividono lo stesso spazio degli indirizzi della memoria del processo padre.
 Tuttavia, ciascun processo ha una copia separata del proprio spazio di indirizzi.

-Separazione del PID: Il PID (Process ID) identifica univocamente ogni processo. Dopo una chiamata a fork, ci sono due processi distinti con PID diversi: il processo padre e il processo figlio.

-Duplicazione dei file descriptor: I file descriptor aperti nel processo padre vengono duplicati nel processo figlio.
 Ciò significa che il processo padre e il processo figlio condivideranno gli stessi file aperti. Questo include i file, i socket, le pipe e altri oggetti di sistema.

*/