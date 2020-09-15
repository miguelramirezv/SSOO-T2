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

void read_file(char* input, Queue* queue){

  FILE *file = fopen(input, "r");
  // Queue* procesos = queue_init();
  char* line[2048];
  char* token;
  int* rafagas;
  int contador;
  int tiempo_max = -1;
  Process* proceso;
  Node* node;
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
    printf("Before append\n");
    // node = list_append_by_deadline(queue -> queue_list, proceso);
    node = list_append_by_start_time(queue -> queues_list, proceso);
    printf("After append\n");
  }
  queue -> queues_list -> tiempo_max = tiempo_max;
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

  Queue* queue = queue_init();
  // printf("AFTER INIT MAIN: %p\n", queue -> queues_list -> head);
  // list_print(queue -> process_list);
  read_file(input_file, queue);
  // printf("\n");
  // list_print(queue -> queues_list);
  printf("TIEMPO MAX: %i\n", queue -> queues_list -> tiempo_max);

  for (int time = 0; time <= queue -> queues_list -> tiempo_max; time++){

    // Revisar si hay que pasar de queue_list a ready
    // printf("QUEUE LIST -> READY\n");
    if (queue -> queues_list -> head != NULL){
      if (queue -> queues_list -> head -> process -> start_time == time){
        printf("Process %s Starts. Time: %i\n", queue -> queues_list -> head -> process -> name, time);
        list_append_by_deadline(queue -> ready_queue, queue -> queues_list -> head -> process);
        Node* node = list_pop(queue -> queues_list); // NO HE TESTEADO
      }
    }

    // printf("CPU -> WAITING OR FINISHED\n");
    // Revisar si hay que pasar de cpu a waiting o finished
    if (queue -> cpu_queue -> head != NULL){
      if (queue -> cpu_queue -> head -> process -> next_stop == time){
        // PROCESO TERMINA. TODO: MAS DE UNO QUE TERMINA EN T
        if (queue -> cpu_queue -> head -> process -> deadline == time){
          // PASA A FINISHED
          queue -> cpu_queue -> head -> process -> finish = 1;
          list_append_by_deadline(queue -> finished_queue, queue -> cpu_queue -> head -> process);
          Node* node = list_pop(queue -> cpu_queue);
          printf("Process %s pasa a FINISHED en Time: %i\n", queue -> cpu_queue -> head -> process -> name, time);
        }
        else {
          // PASA A WAITING
          queue -> cpu_queue -> head -> process -> cantidad_rafagas_completadas++;
          int numero_rafaga = queue -> cpu_queue -> head -> process -> cantidad_rafagas_completadas;
          queue -> cpu_queue -> head -> process -> next_stop = time + queue -> cpu_queue -> head -> process -> list_rafaga[numero_rafaga];
          queue -> cpu_queue -> head -> process -> ultima_rafaga = 1;
          printf("Process %s pasa a WAITING en Time: %i\n", queue -> cpu_queue -> head -> process -> name, time);
          list_append_by_next_stop(queue -> waiting_queue, queue -> cpu_queue -> head -> process);
          Node* node = list_pop(queue -> cpu_queue);
          
        }
      }
    }
// 
// 
    // // // Revisar si hay que pasar de ready a cpu
    if (queue -> ready_queue -> head != NULL){
      if (queue -> cpu_queue -> current_occupancy < cpus){
        if (queue -> ready_queue -> head != NULL){
          int numero_rafaga = queue -> ready_queue -> head -> process -> cantidad_rafagas_completadas;
          int next_stop = time + queue -> ready_queue -> head -> process -> list_rafaga[numero_rafaga];
          if (next_stop > queue -> ready_queue -> head -> process -> deadline){
            // NO ALCANZA A TERMINAR
            queue -> ready_queue -> head -> process -> finish = 0;
            list_append_by_deadline(queue -> finished_queue, queue -> ready_queue -> head -> process);
            Node* node = list_pop(queue -> ready_queue);
          }
          else {
            printf("Proceso: %s pasa A CPU en tiempo %i\n", queue -> ready_queue -> head -> process -> name, time);
            int numero_rafaga = queue -> ready_queue -> head -> process -> cantidad_rafagas_completadas;
            queue -> ready_queue -> head -> process -> next_stop = time + queue -> ready_queue -> head -> process -> list_rafaga[numero_rafaga];
            queue -> ready_queue -> head -> process -> turnos_cpu++;
            queue -> ready_queue -> head -> process -> cantidad_rafagas_completadas++;
            if (queue -> ready_queue -> head -> process -> was_executed == 0){
              queue -> ready_queue -> head -> process -> was_executed = 1;
              queue -> ready_queue -> head -> process -> respose_time = time - queue -> ready_queue -> head -> process -> original_start_time;
            }
            list_append_by_deadline(queue -> cpu_queue, queue -> ready_queue -> head -> process);
            Node* node = list_pop(queue -> ready_queue);
          }

          // printf("Proceso: %s pasa A CPU en tiempo %i\n", queue -> ready_queue -> head -> process -> name, time);
          // list_append_by_deadline(queue -> cpu_queue, queue -> ready_queue -> head);
          // queue -> ready_queue -> head -> process -> turnos_cpu++;
          
          // queue -> ready_queue -> head -> process -> next_stop = time + queue -> ready_queue -> head -> process -> list_rafaga[numero_rafaga];
          // queue -> ready_queue -> head -> process -> cantidad_rafagas_completadas++;
          // Calculo de response time si corresponde
          // if (queue -> ready_queue -> head -> process -> was_executed == 0){
          //   queue -> ready_queue -> head -> process -> was_executed = 1;
          //   queue -> ready_queue -> head -> process -> respose_time = time - queue -> ready_queue -> head -> process -> original_start_time;
          // }
          // Node* node = list_pop(queue -> ready_queue);
          // printf("POP\n");
        }
      }
      else if (queue -> cpu_queue -> current_occupancy == cpus){
        if (queue -> ready_queue -> head != NULL){
          Node* last_node_cpu;
          for (Node* current = queue -> cpu_queue -> head; current; current = current -> next){
            if (current -> next == NULL){
              last_node_cpu = current;
              break;
            }
          }
          if (queue -> ready_queue -> head -> process -> deadline < last_node_cpu -> process -> deadline){
            // Interrumpir
            last_node_cpu -> process -> num_interrupciones++;
            // Registrar donde quedo
            list_append_by_next_stop(queue -> ready_queue, last_node_cpu -> process);
            // list_append_by_deadline(queue -> ready_queue, last_node_cpu -> process);
            Node* popped_node = list_pop_tail(queue -> cpu_queue);
          }
          else if (queue -> ready_queue -> head -> process -> deadline == last_node_cpu -> process -> deadline){
            if (queue -> ready_queue -> head -> process -> pid < last_node_cpu -> process -> pid){
              // Interrumpir
              last_node_cpu -> process -> num_interrupciones++;
              list_append_by_next_stop(queue -> ready_queue, last_node_cpu -> process);
              // list_append_by_deadline(queue -> ready_queue, last_node_cpu -> process);
              Node* popped_node = list_pop_tail(queue -> cpu_queue);
            }
          }
        }
      }
    }
    // printf("WAITING -> READY\n");
    if (queue -> waiting_queue -> head != NULL){
      if (queue -> waiting_queue -> head -> process -> next_stop == time){
        list_append_by_deadline(queue -> ready_queue, queue -> waiting_queue -> head -> process);
        Node* node = list_pop(queue -> waiting_queue);
      }
    }
  }
  // list_print(queue -> queue_list);
  // list_print(queue -> ready_queue);
  // list_print(queue -> cpu_queue);
  return 0;
}
