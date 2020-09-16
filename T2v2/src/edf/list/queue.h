#pragma once
#include "linked_list.h"

struct queue;

typedef struct queue Queue;

struct queue
{
    List* queues_list;
    List* ready_queue;
    List* cpu_queue;
    List* waiting_queue;
    List* finished_queue;
    int num_programs;
};

Queue* queue_init();
