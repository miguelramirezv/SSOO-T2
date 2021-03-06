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

// TURNAROUND TIME: tiempo desde envío hasta término (cuando pase desde cpu a finished, hacer time - start_time_og)
// RESPONSE TIME: tiempo desde llegada a la cola hasta primera ejecución (agregar atributo 1 o 0 if ejecutado alguna vez. Cuando sea ejecutado por primera vez, time - start_time)
// // Implementado. Not tested. 

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
  int tiempo_max = -1;
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
        proceso -> original_start_time = atoi(token);  // Este no va a cambiar 
      }
      else if (contador == 3){
        proceso -> deadline = atoi(token);
        if (proceso -> deadline > tiempo_max){
          tiempo_max = proceso -> deadline;
        }
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
  procesos -> queue_list -> tiempo_max = tiempo_max;
  return procesos;
  // numero_programas = atoi(strtok(NULL, " ")); // no se si necesario
}

Queue* create_waiting(Queue* procesos){

  Node* proceso = procesos -> queue_list -> head;
  Queue* procesos_return = queue_init();
  while (proceso){
    list_append_waiting_time(procesos_return, proceso -> process);
    proceso = proceso -> next;
  }
  
  return procesos_return;
  // numero_programas = atoi(strtok(NULL, " ")); // no se si necesario
}

int main(int argc, char **argv)
{
  for (int i = 0; i < argc; ++i)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
  char* input_file = argv[1];
  char* output_file = argv[2];
  int cpus;
  if (true){    // ESTO HACERLO BIEN REVISAR SI HAY TERCER INPUT
    cpus = 1;
  }
  else {
    cpus = atoi(argv[3]);
  }
  Queue* deadline_queue = read_file(argv[1]);
  Queue* waiting_queue = create_waiting(deadline_queue); //waiting
  Queue* ready_queue = list_init(); //ready
  Queue* finish_queue = list_init(); //finished
  Queue* cpu_queue = list_init(); // running??????

  // list_append cuando queremos meterlo por prioridad de deadline (cpu)
  // list_append_waiting_time cuando queremos ver quien  temina primero

  for (int time = 0; time <= deadline_queue -> queue_list -> tiempo_max; time++){
    bool cambios = true;
    while (cambios){ // entra en algún minuto ? 
      cambios = false;
      if (waiting_queue -> queue_list -> head){
        // Waiting a ready
        if (waiting_queue -> queue_list -> head -> process -> start_time == time){
          Node* waited_node = list_pop(waiting_queue);
          Node* first_ready = list_append(ready_queue, waited_node);
          if (first_ready){
            if (cpu_queue -> queue_list -> current_occupancy < cpus){
              // pasar a estado RUNNING
              list_pop(ready_queue);
              list_append(cpu_queue, first_ready);
              first_ready -> process -> turnos_cpu ++;
              first_ready -> process -> waiting_time += (time - first_ready -> process -> start_time);
              if (first_ready -> process -> was_executed == 0){
                first_ready -> process -> was_executed = 1;
                first_ready -> process -> response_time = time - first_ready -> process -> original_start_time;
              }
            }
            else{
              // Transformo cola de tiempos en cola prioridades
              Queue* temp_cpu_queue = queue_init();
              for (Node* current = cpu_queue -> head; current; current = current -> next){
                list_append(temp_cpu_queue -> queue_list, current);
              }
              // Determinar si corresponde interrumpir o no
              // // Si corresponde interrumpir porque deadline es menor
              if (first_ready -> process -> deadline < temp_cpu_queue -> queue_list -> head -> deadline){
                // Obtener proceso con peor prioridad (último de queue)
                for (Node* current = cpu_queue -> head; current; current = current -> next){
                  // Si current -> next -> next = NULL, entonces current es el penúltimo. Sacamos current -> next (que sería el último).
                  if (!current -> next -> next){
                    list_append(ready_queue -> queue_list, current -> next -> process);
                    current -> next = NULL; // Falta destruir el nodo
                    // Dejar registrado donde quedó ejecutando 
                    list_append(cpu_queue -> queue_list, first_ready -> process); // Agregar a cpu
                    break;
                  }
                }
              }
              // // Caso en que tengan igual deadline. Desempatar por pid
              else if (first_ready -> process -> deadline == temp_cpu_queue -> queue_list -> head -> deadline){
                // Obtener proceso con peor prioridad (último de queue)
                for (Node* current = cpu_queue -> head; current; current = current -> next){
                  // Si current -> next -> next = NULL, entonces current es el penúltimo. Sacamos current -> next (que sería el último).
                  if (!current -> next -> next){
                    // Si first_ready tiene pid más bajo que último de temp_cpu_queue, interrumpir
                    if (first_ready -> process -> pid < current -> next -> process -> pid){
                      list_append(ready_queue -> queue_list, current -> next -> process);
                      current -> next = NULL; // Falta destruir el nodo
                      // Dejar rgistrado donde quedó ejecutando
                      list_append(cpu_queue -> queue_list, first_ready -> process); // Agregar a cpu
                      break;
                    }
                  }
                }

              }
              // // Caso en que no corresponde interrumpir
              else {

              }
              // ver si interrumpir
              // en caso de que sí, 
              // pop ultimo de prioridades
              // que pasa con el tipo que echamos *****
              // pasar de nuevo a cola de tiempos
              // en caso de que no, volver a cola de tiempos
            }
          }
          cambios = true;
        }
      }

      if (cpu_queue -> queue_list -> head){
        if (cpu_queue -> queue_list -> head -> process -> running_time == time){
          cambios = true;
        }
      }
    }
  }
  
  while (finish_queue -> queue_list -> current_occupancy < deadline_queue -> queue_list -> current_occupancy){
    if (waiting_queue -> queue_list -> time < corriendo_cpu -> process -> next_stop){
      Node* ready_node = list_pop(waiting_queue -> queue_list);
      Node* first_ready = list_append(cpu_queue -> queue_list, ready_node);
      if (first_ready){
        if (first_ready -> process -> deadline < corriendo_cpu -> process -> deadline){ //cpus ocupadas∫
          corriendo_cpu -> process -> num_interrupciones += 1; // si es que el que entra ya pasó su deadline
          list_append(cpu_queue -> queue_list, corriendo_cpu); // o pasar a finished?
          corriendo_cpu = first_ready;
        }
      }

    }
    
    else {
      Node* first_ready = list_pop(cpu_queue -> queue_list);
      if (first_ready){
        if (first_ready -> process -> next_stop < first_ready -> process -> cantidad_rafagas){

        }

      }
    }
  }

  Node* corriendo_cpu = list_pop(waiting_queue); // ??????
  return 0;
}
