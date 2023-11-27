#include "minishell.h"

int execute_external_command(char **args)
{
  pid_t pid;
  int status;
  char *path = "/usr/bin/";
  char full_path[256]; // dimensione massima path, va bene?
  char *env[] = {NULL}; //se non metto NULL non funziona. Serviranno valori diversi? Riguarda le variabili d ambiente

  strcpy(full_path, path); // Copia il path nel buffer full_path
  strcat(full_path, args[0]); // Concatena il comando alla fine del path

  pid = fork(); //viene creato un processo figlio
  if (pid == 0) //processo figlio
  {
    if (execve(full_path, args, env) == -1) // esegue il comando nel processo figlio
      perror("error");
    exit(EXIT_FAILURE);
  }
  else if (pid < 0) //errore nel fork
    perror("error");
  else //processo padre
  {
    while (!WIFEXITED(status) && !WIFSIGNALED(status)) // Il ciclo continua finché il processo figlio non termina con successo (WIFEXITED) o viene interrotto da un segnale (WIFSIGNALED).
       waitpid(pid, &status, 0);
  }
  return (1);
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