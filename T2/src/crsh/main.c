#include "args/args.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "list/linked_list.h"
#include "list/queue.h"
#include "process/process.h"
#include <time.h>


// List* pids;
// Args* args;
// void handler(int signal){
//   time_t inicial = time(NULL);
//   time_t final = time(NULL);
//   int running;
//   int end;
//   for(Node* current = pids -> head; current; current = current -> next)
//       {
//         end = waitpid(current -> pid, NULL, WNOHANG);
//         if (!end){
//           kill(current -> pid, 2);
//         }
//       }
//   while (final - inicial < 15){
//     running = 0;
//     for(Node* current = pids -> head; current; current = current -> next)
//       {
//         end = waitpid(current -> pid, NULL, WNOHANG);
//         if (!end){
//           running += 1;
//         }
//       }
//     if (!running){
//       break;
//     }
//     final = time(NULL);
//   }
//   if (running){
//     for(Node* current = pids -> head; current; current = current -> next)
//       {
//         end = waitpid(current -> pid, NULL, WNOHANG);
//         if (!end){
//           kill(current -> pid, 9);
//         }
//       }
//   }
//   args_destroy(args);
//   list_destroy(pids);
//   exit(1);
// }

// void run()
// {
//   args = args_init();
//   pids = list_init();
//   while (true)
//   {
//     signal(SIGINT, handler); //(aquí?)
//     // Leemos la consola
//     args_get(args);

//     // Ejemplo de como utilizar el struct
//     // printf("Command: %s\n", args->command);
//     // printf("Argc:    %i\n", args->argc);
//     // printf("Argv:    ");
//     // for (size_t i = 0; i < args->argc; i++)
//     // {
//     //   printf("%s ", args->argv[i]);
//     // }
//     printf("\n");

//     if (!strcmp(args->command, "crexec")){
//       pid_t pid = fork();
      
//       if (!pid){
//         execvp(args->argv[0], args->argv);
//         if (errno == 2){
//           printf("EXEC FAILED: %s\n", strerror(errno));
//         }
//         exit(1);
//       }

//       else {
//         // printf("soy padre \n");
//         // printf("pid: %i!\n", pid);
//         // printf("nombre archivo: %s\n", args->argv[0]);
//         char* prueba = args->argv[0];
//         // strcpy(prueba, args->argv[0]);
//         list_append(pids, pid, prueba, time(NULL));
//       }
//     }
//     else if (!strcmp(args->command, "crlist")){
//       for(Node* current = pids -> head; current; current = current -> next)
//         {
//           int finish = waitpid(current -> pid, NULL, WNOHANG); //no para y revisa el pid, ret 0 si no ha terminado o pid si termino
//           if (!finish){
//             printf("PID: %i     ;     ", current -> pid);
//             printf("name_exec: %s      ;     ", current -> name);
//             printf("time_running: %ld seconds\n", time(NULL) - current -> time);
//           }

//         }
//     }

//     else if (!strcmp(args->command, "crkill")){
//       // OJO: que el pid tiene que ser mi hijo!
//       int hijo = 0;
//       for(Node* current = pids -> head; current; current = current -> next){
//         if (atoi(args->argv[1]) == current -> pid){
//           hijo = 1;
//         }
//       }
//       if (hijo){
//         kill(atoi(args->argv[1]), atoi(args->argv[0])); // tal vez atoi para señal tbn
//       }
//       else printf("Comando no puede ser ejecutado ya que el proceso no fue creado por crsh\n");
//     }

//     else if (!strcmp(args->command, "crexit")){
//       time_t inicial = time(NULL);
//       time_t final = time(NULL);
//       int running;
//       int end;
//       for(Node* current = pids -> head; current; current = current -> next)
//           {
//             end = waitpid(current -> pid, NULL, WNOHANG);
//             if (!end){
//               kill(current -> pid, 2);
//             }
//           }
//       while (final - inicial < 15){
//         running = 0;
//         for(Node* current = pids -> head; current; current = current -> next)
//           {
//             end = waitpid(current -> pid, NULL, WNOHANG);
//             if (!end){
//               running += 1;
//             }
//           }
//         if (!running){
//           break;
//         }
//         final = time(NULL);
//       }
//       if (running){
//         for(Node* current = pids -> head; current; current = current -> next)
//           {
//             end = waitpid(current -> pid, NULL, WNOHANG);
//             if (!end){
//               kill(current -> pid, 9);
//             }
//           }
//       }
//       args_destroy(args);
//       list_destroy(pids);
//       exit(1);
//     }

//   }

//   args_destroy(args);
// }

Queue* read_file(char* input){
  FILE *file = fopen(input, "r");
  Queue* procesos = queue_init();
  char* line[2048];
  char* token;
  int* rafagas;
  int contador;
  Process* proceso;
  fgets(line, sizeof(line), file);
  int numero_programas = atoi(strtok(line, " "));
  for (int i = 0; i < numero_programas; i++){
    fgets(line, sizeof(line), file);
    token = strtok(line, " ");
    contador = 0;
    while (token != NULL){
      if (contador == 0){
        proceso = process_init(token);

      }
      else if (contador == 1){
        proceso -> pid = atoi(token);
      }
      else if (contador == 2){
        proceso ->start_time = atoi(token);
      }
      else if (contador == 3){
        proceso -> deadline = atoi(token);
      }
      else if (contador == 4){
        proceso -> cantidad_rafagas = atoi(token);
        rafagas = malloc(sizeof(int) * ((proceso -> cantidad_rafagas * 2) - 1) + 1);
      }
      else {
        rafagas[contador - 5] =  atoi(token);
      }
      token = strtok(NULL, " ");
      contador += 1;
    }
    proceso -> list_rafaga = rafagas; // añadir a programas
    printf("Nombre proceso: %s\n", proceso->name);
    list_append(procesos -> queue_list, proceso);
  }
  return procesos;
  // numero_programas = atoi(strtok(NULL, " ")); // no se si necesario
}

int main(int argc, char **argv)
{
  for (int i = 0; i < argc; ++i)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
  read_file(argv[1]);
  return 0;
}
