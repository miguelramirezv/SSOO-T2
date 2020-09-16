#include "queue.h"
#include "linked_list.h"

#include <stdio.h>
#include <stdlib.h>

Queue* queue_init(){
    
    Queue* queue = malloc(sizeof(Queue));
    queue -> queues_list = list_init();      // Sorted by starttime
    queue -> ready_queue = list_init();     // Sorted by deadline
    queue -> cpu_queue = list_init();
    queue -> waiting_queue = list_init();   // Sorted by 
    queue -> finished_queue = list_init();  // Not sorted
    queue -> num_programs = 0;

    // printf("INSIDE QUEUE INIT: %p\n", queue -> queues_list -> head);

    return queue;
}