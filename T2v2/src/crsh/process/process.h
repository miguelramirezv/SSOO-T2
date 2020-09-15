#pragma once

struct process;

typedef struct process Process;

struct process {
    
    int pid;
    char* name;
    int priority; // not yet
    
    // Necesarios para output
    int estado; // not yet
    int turnos_cpu;  // ir actualizando
    int num_interrupciones;  // ir actualizando
    int turnaround_time;  // ir actualizando
    int respose_time;  // ir actualizando
    int waiting_time;  // ir actualizando
    int start_time;
    int cantidad_rafagas;
    int original_start_time;
    int was_executed;
    int deadline;
    int running_time;
    int finish;
    int next_stop; //nro rafaga que voy
    int ultima_rafaga; //0 es waiting y 1 es running
    int* list_rafaga;
};

Process* process_init(char* name);