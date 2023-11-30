#include "minishell.h"

int execute_external_command(char **args, t_shell *shell)
{
  pid_t pid;
  int status;
  char *path_env;
  char full_path[256]; // dimensione massima path, va bene?
  char *env[] = {NULL}; //se non metto NULL non funziona. Serviranno valori diversi? Riguarda le variabili d ambiente
  char *token; 
  
  path_env = getenv("PATH");
  if (path_env == NULL)
  {
    perror("getenv error");
    return (0);
  }
  token = ft_strtok(path_env, ":", shell);
  printf("start token = %s\n", token);
  while (token != NULL)
  {
    strcpy(full_path, token);      // Copia il percorso nel buffer full_path
    strcat(full_path, "/");        // Aggiungi una barra (slash)
    strcat(full_path, args[0]);   // Concatena il comando alla fine del percorso
    printf("full_path = %s\n", full_path);
    printf("token = %s\n", token);
    if (access(full_path, X_OK) == 0)
    {
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
