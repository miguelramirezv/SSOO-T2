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
  queue -> num_programs = numero_programas;

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
        proceso -> enter_ready = atoi(token);
        proceso -> original_start_time = atoi(token);
      }
      else if (contador == 3){
        proceso -> deadline = atoi(token);
        if (proceso -> deadline > tiempo_max){
          tiempo_max = proceso -> deadline;
        }
      }
      else if (contador == 4){
        proceso -> cantidad_rafagas = 2 * atoi(token) - 1;
        rafagas = malloc(sizeof(int) * (proceso -> cantidad_rafagas) + 1);
        // proceso -> cantidad_rafagas = atoi(token);
        // printf("CANTIDAD RAFAGAS: %i\n", proceso -> cantidad_rafagas);
        // rafagas = malloc(sizeof(int) * ((proceso -> cantidad_rafagas * 2) - 1) + 1);
        // printf("MEM ARRAY: %i\n", (proceso -> cantidad_rafagas));
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

void write_file(char *output, Queue *queue){

  FILE *file = fopen(output, "w");
  List* list = queue -> finished_queue;

  for (Node* current = list -> head; current; current = current -> next){
    fprintf(file, "%s,%i,%i,%i,", current->process->name, current->process->turnos_cpu, current->process->num_interrupciones, current->process->turnaround_time);
    fprintf(file, "%i,%i,%i\n", current->process->respose_time, (current->process->time_waiting_queue + current->process->time_ready_queue), current->process->finish);
  }
  fclose(file);

  // fprintf(file, "%s, %i\n", queue -> finished_queue ->);
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
  if (argc < 4){
    cpus = 1;
  }
  else {
    cpus = atoi(argv[3]);
  }
  // if (true){    // ESTO HACERLO BIEN REVISAR SI HAY TERCER INPUT
  //   cpus = 2;
  // }
  // else {
  //   cpus = atoi(argv[3]);
  // }

  Queue* queue = queue_init();
  // printf("AFTER INIT MAIN: %p\n", queue -> queues_list -> head);
  // list_print(queue -> process_list);
  read_file(input_file, queue);
  // printf("\n");
  // list_print(queue -> queues_list);
  // printf("TIEMPO MAX: %i\n", queue -> queues_list -> tiempo_max);
  int time = 0;
  while (queue -> finished_queue -> current_occupancy < queue -> num_programs){
    printf("Time: %i\n", time);
    if (time > 250){
      break;
    }
    // printf("Occupancy Ready: %i\n", queue->ready_queue->current_occupancy);
    // printf("Occupancy CPU: %i\n", queue->cpu_queue->current_occupancy);
    // printf("Occupancy Waiting: %i\n", queue->waiting_queue->current_occupancy);
    // printf("Occupancy Finished: %i\n", queue->finished_queue->current_occupancy);
    //for (int time = 0; time <= queue->queues_list->tiempo_max; time++){
      bool cambios = true;
      while (cambios){
        cambios = false;
        // if (queue -> cpu_queue -> head != NULL){
        //   printf("CPU Head: %s\n", queue -> cpu_queue -> head -> process -> name);
        //   printf("CPU Head Next Stop: %i\n", queue->cpu_queue->head->process->next_stop);
        // }
        // else {
        //   printf("CPU Head: %p\n", queue -> cpu_queue -> head);
        // }
        // Revisar si hay que pasar de queue_list a ready
        // printf("QUEUE LIST -> READY\n");
        if (queue->queues_list->head != NULL)
        {
          if (queue->queues_list->head->process->start_time == time)
          {
            printf("Process %s Starts. Time: %i\n", queue->queues_list->head->process->name, time);
            list_append_by_deadline(queue->ready_queue, queue->queues_list->head->process);
            queue->queues_list->head->process->enter_ready = time;
            Node *node = list_pop(queue->queues_list); // NO HE TESTEADO
            cambios = true;
          }
        }

        // printf("CPU -> WAITING OR FINISHED\n");
        // Revisar si hay que pasar de cpu a waiting o finished
        if (queue->cpu_queue->head != NULL)
        {
          // printf("Time: %i\n", time);
          // printf("NEXT STOP: %i\n", queue -> cpu_queue -> head -> process -> next_stop);
          if (queue->cpu_queue->head->process->next_stop == time)
          {
            // PROCESO TERMINA. TODO: MAS DE UNO QUE TERMINA EN T
            if (queue->cpu_queue->head->process->cantidad_rafagas_completadas == queue->cpu_queue->head->process->cantidad_rafagas - 1)
            {
              // PASA A FINISHED
              printf("Process %s pasa a FINISHED en Time: %i\n", queue->cpu_queue->head->process->name, time);
              if (queue -> cpu_queue -> head -> process -> deadline < time){
                queue->cpu_queue->head->process->finish = 0;
              }
              else {
                queue->cpu_queue->head->process->finish = 1;
              }
              // queue->cpu_queue->head->process->finish = 1;
              queue->cpu_queue->head->process->remaining_interrupcion = -1;
              queue->cpu_queue->head->process->enter_finish = time;
              queue->cpu_queue->head->process->time_running_queue += (time - queue->cpu_queue->head->process->enter_running);
              queue->cpu_queue->head->process->turnaround_time = time - queue->cpu_queue->head->process->original_start_time;
              list_append_by_deadline(queue->finished_queue, queue->cpu_queue->head->process);
              Node *node = list_pop(queue->cpu_queue);
              cambios = true;
            }
            else
            {
              // PASA A WAITING
              queue->cpu_queue->head->process->cantidad_rafagas_completadas++;
              int numero_rafaga = queue->cpu_queue->head->process->cantidad_rafagas_completadas;
              // queue->cpu_queue->head->process->cantidad_rafagas_completadas++;
              queue->cpu_queue->head->process->next_stop = time + queue->cpu_queue->head->process->list_rafaga[numero_rafaga];
              // queue->cpu_queue->head->process->cantidad_rafagas_completadas++;
              queue->cpu_queue->head->process->ultima_rafaga = 1;
              queue->cpu_queue->head->process->remaining_interrupcion = -1;
              queue->cpu_queue->head->process->enter_waiting = time;
              queue->cpu_queue->head->process->time_running_queue += (time - queue->cpu_queue->head->process->enter_running);
              printf("Process %s pasa a WAITING en Time: %i\n", queue->cpu_queue->head->process->name, time);
              //if (time == 113){
              // ßprintf("NEXT STOP %s: %i\n", queue->cpu_queue->head->process->name, queue->cpu_queue->head->process->next_stop);
              //}
              list_append_by_next_stop(queue->waiting_queue, queue->cpu_queue->head->process);
              Node *node = list_pop(queue->cpu_queue);
              cambios = true;
              // if (time == 113)
              // {
              //   printf("WAITING\n");
              //   list_print(queue->waiting_queue);
              //   printf("READY\n");
              //   list_print(queue->ready_queue);
              // }
            }
          }
        }

        // printf("WAITING -> READY\n");
        if (queue->waiting_queue->head != NULL)
        {
          if (queue->waiting_queue->head->process->next_stop == time)
          {
            printf("Proceso: %s pasa A READY en tiempo %i\n", queue->waiting_queue->head->process->name, time);
            queue->waiting_queue->head->process->cantidad_rafagas_completadas++;
            queue->waiting_queue->head->process->enter_ready = time;
            queue->waiting_queue->head->process->time_waiting_queue += (time - queue->waiting_queue->head->process->enter_waiting);
            list_append_by_deadline(queue->ready_queue, queue->waiting_queue->head->process);
            Node *node = list_pop(queue->waiting_queue);
            cambios = true;
          }
        }

        // printf("READY -> CPU");
        if (queue->ready_queue->head != NULL)
        {
          if (queue->cpu_queue->current_occupancy < cpus)
          {
            int numero_rafaga = queue->ready_queue->head->process->cantidad_rafagas_completadas;
            if (queue->ready_queue->head->process->remaining_interrupcion == -1)
            {
              // int next_stop = time + queue->ready_queue->head->process->list_rafaga[numero_rafaga];
              queue->ready_queue->head->process->next_stop = time + queue->ready_queue->head->process->list_rafaga[numero_rafaga];
            }
            else
            {
              queue->ready_queue->head->process->next_stop = time + queue->ready_queue->head->process->remaining_interrupcion;
            }
            printf("Proceso: %s pasa A CPU en tiempo %i\n", queue->ready_queue->head->process->name, time);
            queue->ready_queue->head->process->turnos_cpu++;
            queue->ready_queue->head->process->enter_running = time;
            queue->ready_queue->head->process->time_ready_queue += (queue->ready_queue->head->process->enter_running - queue->ready_queue->head->process->enter_ready);

            // queue->ready_queue->head->process->cantidad_rafagas_completadas++;

            if (queue->ready_queue->head->process->was_executed == 0)
            {
              queue->ready_queue->head->process->was_executed = 1;
              queue->ready_queue->head->process->respose_time = time - queue->ready_queue->head->process->original_start_time;
            }
            list_append_by_next_stop(queue->cpu_queue, queue->ready_queue->head->process);
            Node *node = list_pop(queue->ready_queue);
            cambios = true;

          }

          else if (queue->cpu_queue->current_occupancy == cpus)
          {
            if (queue->ready_queue->head != NULL)
            {
              Node *last_node_cpu = queue->ready_queue->head;
              for (Node *current = queue->cpu_queue->head; current; current = current->next)
              {
                if (current->process->deadline > last_node_cpu->process->deadline)
                {
                  last_node_cpu = current;
                  break;
                }
                else if (current->process->deadline == last_node_cpu->process->deadline)
                {
                  if (current->process->pid > last_node_cpu->process->pid)
                  {
                    last_node_cpu = current;
                    break;
                  }
                }
              }
              if (queue->ready_queue->head->process->deadline < last_node_cpu->process->deadline)
              {
                // Interrumpir
                // printf("INTERRUMPIR -> TIME: %i\n", time);
                // DEFINIR NEXT STOP DEL QUE VA A ENTRAR
                if (queue->ready_queue->head->process->remaining_interrupcion == -1)
                {
                  int numero_rafaga = queue->ready_queue->head->process->cantidad_rafagas_completadas;
                  queue->ready_queue->head->process->next_stop = time + queue->ready_queue->head->process->list_rafaga[numero_rafaga];
                  // printf("NEXT STOP READY HEAD: %i\n", queue->ready_queue->head->process->next_stop);
                }
                else
                {
                  queue->ready_queue->head->process->next_stop = time + queue->ready_queue->head->process->remaining_interrupcion;
                }
                queue->ready_queue->head->process->turnos_cpu++;
                // DEFINIR VALORES DEL QUE VA A SALIR
                last_node_cpu->process->num_interrupciones++;
                last_node_cpu->process->remaining_interrupcion = last_node_cpu->process->next_stop - time;
                last_node_cpu->process->enter_ready = time;
                last_node_cpu->process->time_running_queue += (last_node_cpu->process->enter_ready - last_node_cpu->process->enter_running);
                // printf("NEXT STOP LAST NODE: %i\n", last_node_cpu -> process -> remaining_interrupcion);

                // printf("READY HEAD PROCESS: %p\n", queue -> ready_queue -> head -> process);
                // Registrar donde quedo
                printf("Proceso: %s pasa A READY en tiempo %i", last_node_cpu->process->name, time);
                printf(". Le quedan %i por ejecutar.\n", last_node_cpu->process->remaining_interrupcion);
                printf("Proceso: %s pasa A CPU en tiempo %i\n", queue->ready_queue->head->process->name, time);
                Node *popped_node = list_remove(queue->cpu_queue, last_node_cpu->process->pid);
                Node *node = list_pop(queue->ready_queue);

                node -> process -> enter_running = time;
                node -> process -> time_ready_queue += (node -> process -> enter_running - node -> process -> enter_ready);
                // printf("\nPOPPED NODE: %s\n", popped_node->process->name);

                list_append_by_deadline(queue->ready_queue, last_node_cpu->process);
                list_append_by_next_stop(queue->cpu_queue, node->process);
                cambios = true;

                // list_append_by_next_stop(queue -> cpu_queue, node -> process);
                // list_append_by_deadline(queue -> ready_queue, last_node_cpu -> process);
                // Node* popped_node = list_pop_tail(queue -> cpu_queue);
                // Node* popped_node = list_remove(queue -> cpu_queue, last_node_cpu -> process -> pid);
              }
              else if (queue->ready_queue->head->process->deadline == last_node_cpu->process->deadline)
              {
                if (queue->ready_queue->head->process->pid < last_node_cpu->process->pid)
                {
                  // Interrumpir --> REVISAR
                  if (queue->ready_queue->head->process->remaining_interrupcion == -1)
                  {
                    int numero_rafaga = queue->ready_queue->head->process->cantidad_rafagas_completadas;
                    queue->ready_queue->head->process->next_stop = time + queue->ready_queue->head->process->list_rafaga[numero_rafaga];
                    // printf("NEXT STOP READY HEAD: %i\n", queue->ready_queue->head->process->next_stop);
                  }
                  else
                  {
                    queue->ready_queue->head->process->next_stop = time + queue->ready_queue->head->process->remaining_interrupcion;
                  }
                  // DEFINIR VALORES DEL QUE VA A SALIR
                  last_node_cpu->process->num_interrupciones++;
                  last_node_cpu->process->remaining_interrupcion = last_node_cpu->process->next_stop - time;
                  last_node_cpu->process->enter_ready = time;
                  // printf("NEXT STOP LAST NODE: %i\n", last_node_cpu -> process -> remaining_interrupcion);

                  // printf("READY HEAD PROCESS: %p\n", queue -> ready_queue -> head -> process);
                  // Registrar donde quedo
                  printf("Proceso: %s pasa A READY en tiempo %i", last_node_cpu->process->name, time);
                  printf(". Le quedan %i por ejecutar.\n", last_node_cpu->process->remaining_interrupcion);
                  printf("Proceso: %s pasa A CPU en tiempo %i\n", queue->ready_queue->head->process->name, time);
                  Node *popped_node = list_remove(queue->cpu_queue, last_node_cpu->process->pid);
                  Node *node = list_pop(queue->ready_queue);

                  node->process->enter_running = time;
                  // printf("\nPOPPED NODE: %s\n", popped_node->process->name);

                  list_append_by_deadline(queue->ready_queue, last_node_cpu->process);
                  list_append_by_next_stop(queue->cpu_queue, node->process);
                  cambios = true;

                  // last_node_cpu->process->num_interrupciones++;
                  // last_node_cpu->process->remaining_interrupcion = last_node_cpu->process->next_stop - time;
                  // list_append_by_deadline(queue->ready_queue, last_node_cpu->process);
                  // list_append_by_deadline(queue -> ready_queue, last_node_cpu -> process);
                  // Node* popped_node = list_pop_tail(queue -> cpu_queue);
                  // Node *popped_node = list_remove(queue->cpu_queue, last_node_cpu->process->pid);
                  // cambios = true;
                }
              }
            }
          }
        }
        //}
        // time++;
      }
      time++;
      //}
  }
  
  list_print_final(queue -> finished_queue);
  write_file(output_file, queue);
  return 0;
}
