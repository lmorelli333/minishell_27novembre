#include "minishell.h"

int execute_external_command(char **args, t_shell *shell)
{
  pid_t pid;
  int status;
  char *path_env = getenv("PATH");
  char full_path[256]; // dimensione massima path, va bene?
  char *env[] = {NULL}; //se non metto NULL non funziona. Serviranno valori diversi? Riguarda le variabili d ambiente

  if (path_env == NULL)
  {
     perror("getenv error");
    return (0);
  }
  char *token = ft_strtok(path_env, ":", shell);
  printf("%s  CODDIO\n", token);
  while (token != NULL)
  {
    strcpy(full_path, token);      // Copia il percorso nel buffer full_path
    strcat(full_path, "/");        // Aggiungi una barra (slash)
    strcat(full_path, args[0]);    // Concatena il comando alla fine del percorso
    printf("%s  CAMADONNA\n", full_path);
    if (access(full_path, X_OK) == 0)
    {
      printf("%s\n", full_path);
      pid = fork(); //viene creato un processo figlio
      if (pid == 0) //processo figlio
      {
        if (execve(full_path, args, env) == -1) // esegue il comando nel processo figlio
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
    token = ft_strtok(NULL, ":", shell);
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
/*
#include "minishell.h"

// Funzione di split personalizzata
static char **split_string(const char *str, char delimiter)
{
    char **result = NULL;
    size_t count = 0;
    size_t start = 0;
    size_t token_index = 0;
    size_t i;

    i = 0;
    // Conta il numero di token
    while (str[i]) 
    {
        if (str[i] == delimiter)
            ++count;
        i++;
    }

    // Alloca spazio per l'array di stringhe
    result = malloc((count + 2) * sizeof(char *));
    if (result == NULL)
    {
        perror("malloc error");
        exit(EXIT_FAILURE);
    }
    i = 0;
    // Trova i token e crea le stringhe
    while (str[i])
    {
        if (str[i] == delimiter || str[i + 1] == '\0')
        {
            size_t end = (str[i] == delimiter) ? i : i + 1;
            size_t token_length = end - start;
            result[token_index] = malloc((token_length + 1) * sizeof(char));
            if (result[token_index] == NULL)
            {
                perror("malloc error");
                exit(EXIT_FAILURE);
            }
            strncpy(result[token_index], str + start, token_length);
            result[token_index][token_length] = '\0';
            ++token_index;
            start = i + 1;
        }
        ++i;
    }

    result[count + 1] = NULL; // Ultimo elemento dell'array è NULL
    return result;
}

int execute_external_command(char **args)
{
    pid_t pid;
    int status;
    char *path_env = getenv("PATH");

    if (path_env == NULL) {
        perror("getenv error");
        return 0;
    }

    char **tokens = split_string(path_env, ':');
    if (tokens == NULL) {
        return 0;
    }

    size_t token_index = 0;
    while (tokens[token_index] != NULL) {
        char *full_path = malloc(strlen(tokens[token_index]) + strlen(args[0]) + 2);
        if (full_path == NULL) {
            perror("malloc error");
            exit(EXIT_FAILURE);
        }

        sprintf(full_path, "%s/%s", tokens[token_index], args[0]);

        printf("%s\n", full_path);

        if (access(full_path, X_OK) == 0) {
            pid = fork();
            if (pid == 0) {
                execve(full_path, args, NULL);
                perror("execve error");
                exit(EXIT_FAILURE);
            } else if (pid < 0) {
                perror("fork error");
                free(full_path);
                return 0;
            } else {
                if (waitpid(pid, &status, 0) == -1) {
                    perror("waitpid error");
                    free(full_path);
                    return 0;
                }

                free(full_path);

                if (WIFEXITED(status)) {
                    int exit_status = WEXITSTATUS(status);
                    printf("Child process exited with status %d\n", exit_status);
                } else if (WIFSIGNALED(status)) {
                    int signal_number = WTERMSIG(status);
                    printf("Child process terminated by signal %d\n", signal_number);
                }

                free(tokens);
                return 1;
            }
        }

        free(full_path);
        ++token_index;
    }

    perror("error, the command doesn't exist or you don't have the necessary permissions");
    free(tokens);
    return 0;
}


*/