#pragma once

struct process;

typedef struct process Process;

struct process {
    
    int pid;
    char* name;
    int priority;
    
    // Necesarios para output
    int turnos_cpu;
    int num_interrupciones;
    int turnaround_time;
    int respose_time;
    int waiting_time;
    int deadline;
};

Process* process_init(int pid, char* name, int priority);