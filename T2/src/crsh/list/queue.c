#include "queue.h"
#include "linked_list.h"

#include <stdio.h>
#include <stdlib.h>

Queue* queue_init(){
    
    Queue* queue = malloc(sizeof(queue));
    queue -> queue_list = list_init();

}

