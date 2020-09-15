#include "process.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Process* process_init(char* name){

    Process* process = malloc(sizeof(Process));
    process -> pid = 0;
    process -> name = malloc(strlen(name) + 1);
    strcpy(process -> name, name);
    process -> priority = 0;
    process -> next_stop = 1000000;
    process -> estado = 0;
    process -> turnos_cpu = 0;
    process -> num_interrupciones = 0;
    process -> turnaround_time = 0;
    process -> respose_time = 0;
    process -> waiting_time = 0;
    process -> start_time = 0;
    process -> original_start_time = 0;
    process -> was_executed = 0;
    process -> running_time = 1000000;
    process -> cantidad_rafagas = 0; // Numero total rafagas
    process -> cantidad_rafagas_completadas = 0;
    process -> ultima_rafaga = 0; //0 es waiting y 1 es running
    process -> deadline = 0;
    process -> finish = 0; // 1 si logra terminar, 0 si no

    return process;

}

