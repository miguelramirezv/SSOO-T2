#include "process.h"

#include <stdio.h>
#include <stdlib.h>

Process* process_init(int pid, char* name, int priority){

    Process* process = malloc(sizeof(Process));

    process -> pid = pid;
    process -> name = name;
    process -> priority = priority;

    process -> turnos_cpu = 0;
    process -> num_interrupciones = 0;
    process -> turnaround_time = 0;
    process -> respose_time = 0;
    process -> waiting_time = 0;
    process -> deadline = 0; // 1 si logra terminar, 0 si no

}

