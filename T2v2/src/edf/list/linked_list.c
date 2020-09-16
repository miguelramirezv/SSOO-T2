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
  list -> time = 100000000;

  return list;
}

Node* list_append_by_deadline(List* list, Process* process)
{
  // printf("HEY \n");
  Node* node = malloc(sizeof(Node));
  node -> process = process;
  node -> next = NULL;
  // printf("B4 LOGIC\n");
  // printf("HEAD: %p\n", list -> head);
  // printf("HEAD: %p\n", list -> head -> process -> name);

  //return node;

  // // Caso en que nuevo nodo queda en primer lugar de cola
  if (!list -> head){
    // printf("Im here\n");
    list -> head = node;
    // return node;
  }
  // return node;
  // printf("PASE IF\n");
  else if (node -> process -> deadline < list -> head -> process -> deadline){
      // printf("else if 1\n");
      node -> next = list -> head;
      list -> head = node;
      // return node;
  }
  // return node;
  // // Caso en que tiene mismo deadline que primer lugar de cola
  else if (node -> process -> deadline == list -> head -> process -> deadline){
      // printf("else if 2\n");
      // Caso en que proceso queda en primer lugar por tener menor pid
      if (node -> process -> pid < list -> head -> process -> pid){
          node -> next = list -> head;
          list -> head = node;
          return node;
      }
      // Caso en que proceso no queda en primer lugar. Hay que revisar donde queda.
      else {
          // printf("FOR LOOP 1 \n");
          for (Node* current = list -> head; current; current = current -> next){
            if (current -> next){
              if (node -> process -> deadline == current -> next -> process -> deadline){
                if (node -> process -> pid < current -> next -> process -> pid){
                  node -> next = current -> next;
                  current -> next = node;
                  break;
                }
              }
              else {
                node -> next = current -> next;
                current -> next = node;
                break;
              }
              // if (node -> process -> pid < current -> next -> process -> pid){
              //     node -> next = current -> next;
              //     current -> next = node;
              //     break;
              // }
            }
            else{
              current -> next = node;
              node -> next = NULL;
            }
          }
      }
  }
  // Caso en que no tiene mismo deadline que primero de la cola
  else {
      // printf("FOR LOOP 2 \n");
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
      // return NULL;
  }
  // printf("END\n");
  list -> current_occupancy++;
  return node;
}

Node* list_append_by_start_time(List* list, Process* process){

  // printf("ENNTRE\n");

  Node* node = malloc(sizeof(Node));
  node -> process = process;
  node -> next = NULL;

  if (!list -> head){
    list -> head = node;
  }
  else if (node -> process -> start_time < list -> head -> process -> start_time){
    node -> next = list -> head;
    list -> head = node;
  }
  else if (node -> process -> start_time == list -> head -> process -> start_time){
    if (node -> process -> pid < list -> head -> process -> pid){
      node -> next = list -> head;
      list -> head = node;
    }
    else {
      for (Node* current = list -> head; current; current = current -> next){
        if (node -> process -> start_time == current -> process -> start_time){
          if (node -> process -> pid < current -> process -> pid){
            node -> next = current -> next;
            current -> next = node;
            break;
          }
        }
        else {
          node -> next = current -> next;
          current -> next = node;
          break;
        }
        
        // if (node -> process -> pid < current -> next -> process -> pid){
        //   node -> next = current -> next;
        //   current -> next = node;
        //   break;
        // }
      }
    }
  }
  else {
    for (Node* current = list -> head; current; current = current -> next){
      if (!current -> next){
        node -> next = NULL;
        current -> next = node;
        break;
      }
      else if (node -> process -> start_time < current -> next -> process -> start_time){
        node -> next = current -> next;
        current -> next = node;
        break;
      }
      else if (node -> process -> start_time == current -> next -> process -> start_time){
        if (node -> process -> pid < current -> next -> process -> pid){
          node -> next = current -> next;
          current -> next = node;
          break;
        }
      }
    }
  }

  list -> current_occupancy++;
  return node;
};

Node* list_append_by_next_stop(List* list, Process* process){

  Node* node = malloc(sizeof(Node));
  node -> process = process;
  node -> next = NULL;

  if (!list -> head){
    list -> head = node;
  }
  else if (node -> process -> next_stop < list -> head -> process -> next_stop){
    node -> next = list -> head;
    list -> head = node;
  }
  else if (node -> process -> next_stop == list -> head -> process -> next_stop){
    if (node -> process -> pid < list -> head -> process -> pid){
      node -> next = list -> head;
      list -> head = node;
    }
    else {
      for (Node* current = list -> head; current; current = current -> next){
        if (current -> next){
          if (node -> process -> next_stop == current -> next -> process -> next_stop){
            if (node -> process -> pid < current -> next -> process -> pid){
              node -> next = current -> next;
              current -> next = node;
              break;
            }
          }
          else {
            node -> next = current -> next;
            current -> next = node;
            break;
          }
          // if (node -> process -> pid < current -> next -> process -> pid){
          //   node -> next = current -> next;
          //   current -> next = node;
          //   break;
          // }
        }
        else {
          node -> next = NULL;
          current -> next = node;
          break;
        }
      }
    }
  }
  else {
    for (Node* current = list -> head; current; current = current -> next){
      if (!current -> next){
        node -> next = NULL;
        current -> next = node;
        break;
      }
      else if (node -> process -> next_stop < current -> next -> process -> next_stop){
        node -> next = current -> next;
        current -> next = node;
        break;
      }
      else if (node -> process -> next_stop == current -> next -> process -> next_stop){
        if (node -> process -> pid < current -> next -> process -> pid){
          node -> next = current -> next;
          current -> next = node;
          break;
        }
      }
    }
  }

  list -> current_occupancy++;
  return node;

};


// Node* list_append_waiting_time(List* list, Process* process)
// {
//   Node* node = malloc(sizeof(Node));
//   node -> process = process;
//   node -> next = NULL;
// 
//   // Caso en que nuevo nodo queda en primer lugar de cola
//   if (list -> head == NULL){
//     list -> head = node;
//     list -> time = node -> process -> start_time;
//     return node;
//   }
//   else if (node -> process -> start_time < list -> head -> process -> start_time){
//       node -> next = list -> head;
//       list -> head = node;
//       list -> time = node -> process -> start_time;
//       return node;
//   }
//   // Caso en que tiene mismo start_time que primer lugar de cola
//   else if (node -> process -> start_time == list -> head -> process -> start_time){
//       // Caso en que proceso queda en primer lugar por tener menor deadline
//       if (node -> process -> deadline < list -> head -> process -> deadline){
//           node -> next = list -> head;
//           list -> head = node;
//           list -> time = node -> process -> start_time;
//           return node;
//       }
//       else if ((node -> process -> deadline == list -> head -> process -> deadline) && (node -> process -> pid < list -> head -> process -> pid)){
//           node -> next = list -> head;
//           list -> head = node;
//           list -> time = node -> process -> start_time;
//           return node;
//       }
//       // Caso en que proceso no queda en primer lugar. Hay que revisar donde queda.
//       else {
//           for (Node* current = list -> head; current; current = current -> next){
//               if (node -> process -> deadline < current -> next -> process -> deadline){
//                   node -> next = current -> next;
//                   current -> next = node;
//                   return NULL;
//                   break;
//               }
//               else if (node -> process -> deadline == current -> next -> process -> deadline){
//                 for (Node* current2 = current; current2; current2 = current2 -> next){
//                   if (node -> process -> pid < current2 -> next -> process -> pid){
//                     node -> next = current2 -> next;
//                     current2 -> next = node;
//                     return NULL;
//                     break;
//                   }
//                 }
//               }
//           }
//       }
//   }
//   // Caso en que no tiene mismo start_time que primero de la cola
//   else {
//       for (Node* current = list -> head; current; current = current -> next){
//           if (current -> next == NULL){
//             node -> next = NULL;
//             current -> next = node;
//             break;
// 
//           }
//           else if (node -> process -> start_time < current -> next -> process -> start_time){
//               // Entra aca
//               node -> next = current -> next;
//               current -> next = node;
//               break;
//           }
//           else if (node -> process -> start_time == current -> next -> process -> start_time){
//               // Revisar por pid
//               if (node -> process -> deadline < current -> next -> process -> deadline){
//                   node -> next = current -> next;
//                   current -> next = node;
//                   break;
//               }
//           }
//       }
//   }
//   list -> current_occupancy++;
// }
/* Saca primer elemento de la lista */
Node* list_pop(List* list){
  
  // FALTA REVISAR SI ES LA UlTIMA
  Node* node = list -> head; /* First element */
  // printf("HEAD INSIDE PROCESS: %p\n", node -> process);
  if (node == NULL){
    return NULL;
  }
  Node* new_head = list -> head -> next;  /* Second element, new head */
  // printf("NEW HEAD: %p\n", new_head);
  // list -> current_occupancy--;

  if (!new_head){
    // printf("ENTRE\n");
    list -> time = 1000000;
    list -> head = NULL;
    // return node;
  }
  else {
    list -> head = new_head;
  }
  // list -> head = new_head;   /* Assign new head to list */
  list -> current_occupancy--;

  // printf("Removed Node Process: %p\n", node -> process);

  return node;
};

Node* list_remove(List* list, int pid){

  Node* node = list -> head;
  if (node == NULL){
    return NULL;
  }

  if (node -> process -> pid == pid){
    if (node -> next){
      list -> head = node -> next;
    }
    else {
      list -> head = NULL;
    }
  }
  else {
    for (Node* current = list -> head; current; current = current -> next){
      if (current -> next){
        if (current -> next -> process -> pid == pid){
          if (current -> next -> next){
            node = current -> next;
            current -> next = current -> next -> next;
            break;
          }
          else {
            node = current -> next;
            current -> next = NULL;
          }
        }
      }
    }
  }
  list -> current_occupancy--;
  return node;
}

Node* list_pop_tail(List* list){

  Node* tail;
  
  if (list -> head){
    for (Node* current = list -> head; current; current = current -> next){
      if (current -> next){
        if (!current -> next -> next){
          tail = current -> next;
          current -> next = NULL;
        }  
      }
    }
  }
  return tail;
}

/** Imprime todos los elementos de la lista */
void list_print(List* list)
{
  printf("\n");
  printf("Current Head: %p\n", list -> head);
  printf("Current Tail: %p\n", list -> tail);
  printf("Current Length: %i\n", list -> current_occupancy);
  for(Node* current = list -> head; current; current = current -> next)
  {
    printf("%s\n", current -> process -> name); /* -> value*/
  }
}

void list_print_final(List *list)
{
  printf("\n");
  // printf("Current Head: %p\n", list->head);
  // printf("Current Tail: %p\n", list->tail);
  // printf("Current Length: %i\n", list->current_occupancy);
  for (Node *current = list->head; current; current = current->next)
  {
    printf("Name: %s  Turnos CPU: %i    Interrupciones: %i\n", current->process->name, current->process->turnos_cpu, current->process->num_interrupciones); /* -> value*/
    printf("Turnaround: %i   Response: %i    Waiting: %i     Deadline: %i\n", current->process->turnaround_time, current->process->respose_time, (current->process->time_waiting_queue+current->process->time_ready_queue), current->process->finish);
    printf("\n");
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
