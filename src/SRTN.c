#include "../lib/SRTN.h"
#include <stdlib.h>
#include <stdio.h>

static queue_object *SRTN_queue;

process *SRTN_tick(process *running_process) {
    if (!running_process || running_process->time_left == 0) {
        running_process = queue_poll(SRTN_queue); 
    }
    if (running_process) {
        running_process->time_left--; 
    }
    return running_process;
}

//standard startup
int SRTN_startup() {
    SRTN_queue = new_queue(); 
    return SRTN_queue ? 0 : 1; 
}

static int append_to_queue(process *new_process, queue_object *queue) {
    if (!queue || !new_process) return 1; 
    queue_object *new_node = malloc(sizeof(queue_object));
    if (!new_node) return 1; 

    new_node->object = new_process; 
    new_node->next = NULL;

    // korrekte position für prozess finden, abh. von restzeit 
    queue_object *cursor = queue;
    while (cursor->next && ((process *)cursor->next->object)->time_left <= new_process->time_left) {
        cursor = cursor->next;
    }
    new_node->next = cursor->next;
    cursor->next = new_node;

    return 0;
}

process *SRTN_new_arrival(process *arriving_process, process *running_process) {
    if (arriving_process) {
        append_to_queue(arriving_process, SRTN_queue); // ankommmenden prozess hinzufügen
    }

    
    if (!running_process || (arriving_process && arriving_process->time_left < running_process->time_left)) {
        if (running_process && running_process->time_left > 0) {
            append_to_queue(running_process, SRTN_queue); 
        }
        running_process = queue_poll(SRTN_queue); // prozess mit kürzester restzeit holen
    }

    return running_process;
}

void SRTN_finish() {
    free_queue(SRTN_queue); 
}
