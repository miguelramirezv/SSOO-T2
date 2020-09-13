#include "process.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Process* process_init(char* name){

    Process* process = malloc(sizeof(Process));
    printf("entra4-+\n");
    process -> pid = 0;
    printf("entra4-+```````++++\n");
    process -> name = malloc(strlen(name) + 1);
    strcpy(process -> name, name);
    process -> priority = 0;
    printf("entra4-+-\n");

    process -> estado = 0;
    process -> turnos_cpu = 0;
    process -> num_interrupciones = 0;
    process -> turnaround_time = 0;
    process -> respose_time = 0;
    process -> waiting_time = 0;
    process -> start_time = 0;
    process -> cantidad_rafagas = 0;
    process -> ultima_rafaga = 0; //0 es waiting y 1 es running
    process -> deadline = 0;
    process -> finish = 0; // 1 si logra terminar, 0 si no

    return process;

}
