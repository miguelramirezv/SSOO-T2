
  // Queue* deadline_queue = read_file(argv[1]);
  // Queue* waiting_queue = create_waiting(deadline_queue); //waiting
  // Queue* ready_queue = list_init(); //ready
  // Queue* finish_queue = list_init(); //finished
  // Queue* cpu_queue = list_init(); // running??????

  // list_append cuando queremos meterlo por prioridad de deadline (cpu)
  // list_append_waiting_time cuando queremos ver quien  temina primero

  // for (int time = 0; time <= queue -> process_list -> tiempo_max; time++){

    // Ver si algun proceso parte -> Pasar de process_list a READY
    // Ver si alguno pasa de cpu a finished
    // Ver si alguno pasa de cpu a waiting
    // Ver si alguno pasa de waiting a ready
    // Ver si alguno pasa de ready a cpu (interrumpe o no)
  // }

  // for (int time = 0; time <= deadline_queue -> queue_list -> tiempo_max; time++){
  //   
  //   bool cambios = true;
  //   while (cambios){
  //     cambios = false;
  //     if (waiting_queue -> queue_list -> head){
  //       if (waiting_queue -> queue_list -> head -> process -> start_time == time){
  //         Node* waited_node = list_pop(waiting_queue);
  //         Node* first_ready = list_append(ready_queue, waited_node);
  //         if (first_ready){
  //           if (cpu_queue -> queue_list -> current_occupancy < cpus){
  //             // pasar a estado RUNNING
  //             list_pop(ready_queue);
  //             list_append(cpu_queue, first_ready);
  //             first_ready -> process -> turnos_cpu ++;
  //             first_ready -> process -> waiting_time += (time - first_ready -> process -> start_time);
// 
  //           }
  //           else{
  //             // transformar cola de tiempos en cola prioridades
  //             // 
  //             // ver si interrumpir
  //             // en caso de que sí, 
  //             // pop ultimo de prioridades
  //             // que pasa con el tipo que echamos *****
  //             // pasar de nuevo a cola de tiempos
  //             // en caso de que no, volver a cola de tiempos
  //           }
  //         }
  //         cambios = true;
  //       }
  //     }
// 
  //     if (cpu_queue -> queue_list -> head){
  //       if (cpu_queue -> queue_list -> head -> process -> running_time == time){
  //         cambios = true;
  //       }
  //     }
  //   }
  // }
  // 
  // while (finish_queue -> queue_list -> current_occupancy < deadline_queue -> queue_list -> current_occupancy){
  //   if (waiting_queue -> queue_list -> time < corriendo_cpu -> process -> next_stop){
  //     Node* ready_node = list_pop(waiting_queue -> queue_list);
  //     Node* first_ready = list_append(cpu_queue -> queue_list, ready_node);
  //     if (first_ready){
  //       if (first_ready -> process -> deadline < corriendo_cpu -> process -> deadline){ //cpus ocupadas∫
  //         corriendo_cpu -> process -> num_interrupciones += 1; // si es que el que entra ya pasó su deadline
  //         list_append(cpu_queue -> queue_list, corriendo_cpu); // o pasar a finished?
  //         corriendo_cpu = first_ready;
  //       }
  //     }
// 
  //   }
  //   
  //   else {
  //     Node* first_ready = list_pop(cpu_queue -> queue_list);
  //     if (first_ready){
  //       if (first_ready -> process -> next_stop < first_ready -> process -> cantidad_rafagas){
// 
  //       }
// 
  //     }
  //   }
  // }
// 
  // Node* corriendo_cpu = list_pop(waiting_queue); // ??????



// Queue* create_waiting(Queue* procesos){
// 
//   printf("RUN\n");
// 
//   Node* proceso = procesos -> queue_list -> head;
//   Queue* procesos_return = queue_init();
//   while (proceso){
//     list_append_waiting_time(procesos_return, proceso -> process);
//     proceso = proceso -> next;
//   }
//   
//   return procesos_return;
//   // numero_programas = atoi(strtok(NULL, " ")); // no se si necesario
// }



////////////////////////// 

    // Revisar si hay que pasar de ready a cpu --> CPU NO LLENA O INTERRUPCION
    // if (queue -> ready_queue -> head != NULL){
    //   // CPU no llena
    //   if (queue -> cpu_queue -> current_occupancy < cpus){
    //     list_append_by_deadline(queue -> cpu_queue, queue -> ready_queue -> head -> process);
    //     Node* node = list_pop(queue -> ready_queue);
    //   }
    //   // Revisar si hay que interrumpir
    // }