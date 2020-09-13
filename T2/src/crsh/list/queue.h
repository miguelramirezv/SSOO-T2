#pragma once
#include "linked_list.h"

struct queue;

typedef struct queue Queue;

struct queue
{
    List* queue_list;
};

Queue* queue_init();
