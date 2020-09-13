#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>


/** Inicializa una lista vacía */
List* list_init()
{
  List* list = malloc(sizeof(List));
  list -> head = NULL;
  list -> tail = NULL;
  list -> current_occupancy = 0;

  return list;
}

void list_append(List* list, Process* process)
{
  Node* node = malloc(sizeof(Node));
  node -> process = process;
  node -> next = NULL;

  // Caso en que nuevo nodo queda en primer lugar de cola
  if (list -> head == NULL){
    list -> head = node;
  }
  else if (node -> process -> deadline < list -> head -> process -> deadline){
      node -> next = list -> head;
      list -> head = node;
  }
  // Caso en que tiene mismo deadline que primer lugar de cola
  else if (node -> process -> deadline == list -> head -> process -> deadline){
      // Caso en que proceso queda en primer lugar por tener menor pid
      if (node -> process -> pid < list -> head -> process -> pid){
          node -> next = list -> head;
          list -> head = node;
      }
      // Caso en que proceso no queda en primer lugar. Hay que revisar donde queda.
      else {
          for (Node* current = list -> head; current; current = current -> next){
              if (node -> process -> pid < current -> next -> process -> pid){
                  node -> next = current -> next;
                  current -> next = node;
                  break;
              }
          }
      }
  }
  // Caso en que no tiene mismo deadline que primero de la cola
  else {
      for (Node* current = list -> head; current; current = current -> next){
          if (current -> next == NULL){
            node -> next = NULL;
            current -> next = node;
            break;

          }
          else if (node -> process -> deadline < current -> next -> process -> deadline){
              // Entra aca
              node -> next = current -> next;
              current -> next = node;
              break;
          }
          else if (node -> process -> deadline == current -> next -> process -> deadline){
              // Revisar por pid
              if (node -> process -> pid < current -> next -> process -> pid){
                  node -> next = current -> next;
                  current -> next = node;
                  break;
              }
          }
      }
  }
  list -> current_occupancy++;
}

/* Saca primer elemento de la lista */
Node* list_pop(List* list){
  
  Node* node = list -> head; /* First element */
  Node* new_head = list -> head -> next;  /* Second element, new head */

  list -> head = new_head;   /* Assign new head to list */
  list -> current_occupancy--;

  printf("Removed Node: %p\n", node);

  return node;
};

/** Imprime todos los elementos de la lista */
void list_print(List* list)
{
  printf("\n");
  printf("Current Head: %p\n", list -> head);
  printf("Current Tail: %p\n", list -> tail);
  printf("Current Length: %i\n", list -> current_occupancy);
  for(Node* current = list -> head; current; current = current -> next)
  {
    printf("%p\n", current); /* -> value*/
  }
}

/** Libera todos los recursos asociados a esta lista */
void list_destroy(List* list)
{
  // Liberamos los nodos
  if(list -> head)
  {
    Node* curr = list -> head -> next;
    Node* prev = list -> head;

    while(curr)
    {
      free(prev);
      prev = curr;
      curr = curr -> next;
    }
    free(prev);
  }
  // Liberamos la lista en si
  free(list);
}
