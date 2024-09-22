#include "../lib/HRRN.h"
#include "../lib/queue.h"
#include <stdlib.h>
#include <stdio.h>

static queue_object *HRRN_queue;
static unsigned int current_time = 0;

// Sucht den nächsten Prozess 
process *find_next(queue_object *queue) {
    if (!queue || !queue->next) return NULL; 

    queue_object *current = queue->next, *previous = queue;
    queue_object *best_prev = NULL, *best_node = NULL;
    double highest_rr = -1.0;

    // suche nach Prozess mit größtem RR
    while (current) {
        process *proc = (process *)current->object;
        unsigned int wait_time = current_time - proc->start_time;
        double rr = (double)(wait_time + proc->time_left) / proc->time_left;

        if (rr > highest_rr) {
            highest_rr = rr;
            best_prev = previous;
            best_node = current;
        }

        previous = current;
        current = current->next;
    }

    // entfernt Prozess
    if (best_node) {
        best_prev->next = best_node->next;
        process *selected_process = (process *)best_node->object;
        free(best_node);
        return selected_process;
    }

    return NULL;
}

process *HRRN_tick(process *running_process) {
    current_time++; 
    if (!running_process || running_process->time_left == 0) {
        running_process = find_next(HRRN_queue); 
    }
    if (running_process) running_process->time_left--; 

    return running_process;
}

int HRRN_startup() {
    HRRN_queue = new_queue(); 
    if (!HRRN_queue) return 1; 
    current_time = 0; 
    return 0;
}

// ankommenden Prozess warteschlange hinzufügen
process *HRRN_new_arrival(process *arriving_process, process *running_process) {
    if (arriving_process) queue_add(arriving_process, HRRN_queue); 
    return running_process; 
}

// Scheduling beenden und Speicher frei geben
void HRRN_finish() {
    free_queue(HRRN_queue); 
}
