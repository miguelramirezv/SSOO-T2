#pragma once

struct process;

typedef struct process Process;

struct process {
    
    int pid;
    char* name;
    int priority;
    
    // Necesarios para output
    int estado;
    int turnos_cpu;
    int num_interrupciones;
    int turnaround_time;
    int respose_time;
    int waiting_time;
    int start_time;
    int cantidad_rafagas;
    int deadline;
    int finish;
    int ultima_rafaga; //0 es waiting y 1 es running
    int* list_rafaga;
};

Process* process_init(char* name);