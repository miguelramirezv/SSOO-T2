#pragma once
#include "../process/process.h"

struct linked_list_node;

typedef struct linked_list_node Node;

struct linked_list_node
{
  Process* process;
  Node* next;
};

struct linked_list;

typedef struct linked_list List;

struct linked_list
{
  Node* head;

  Node* tail;

  int current_occupancy;

  int time;

  int tiempo_max;
};


/** Inicializa una lista vacía */
List* list_init();
/** Inserta un nuevo elemento al final de la lista */
Node* list_append(List* list, Process* process);

Node* list_append_waiting_time(List* list, Process* process);

Node* list_pop(List* list);
/** Imprime todos los elementos de la lista */
void list_print(List* list);
/** Libera todos los recursos asociados a esta lista */
void list_destroy(List* list);
