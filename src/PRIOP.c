#include "../lib/PRIOP.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * PRIOP ist als Beispiel vorgegeben
 */
static queue_object *PRIOP_queue;

process *PRIOP_tick(process *running_process) {
    // prüfen ob prozess läuft
    if (!running_process || !running_process->time_left) running_process = queue_poll(PRIOP_queue);
    if (running_process) running_process->time_left--;
    return running_process;
}

// startup
int PRIOP_startup() {
    return (PRIOP_queue = new_queue()) ? 0 : 1;
}

// neuen prozess hinzufügen 
int add_queue_priop(process *new_process, queue_object *queue) {
    if (!queue || !new_process) return 1;
    // neuen knoten für prozess erstellen
    queue_object* new_node = malloc(sizeof(queue_object));
    new_node->object = new_process; new_node->next = NULL;

    // korrekte position in queue suchen (nach prio)
    queue_object* current = queue;
    while (current->next && ((process*)current->next->object)->priority >= new_process->priority)
        current = current->next;
    // knoten an richtiger stelle einfügen
    new_node->next = current->next;
    current->next = new_node;
    return 0;
}

// neuer prozess kommt an
process *PRIOP_new_arrival(process *arriving_process, process *running_process) {
    if (arriving_process) add_queue_priop(arriving_process, PRIOP_queue);
    if (!running_process || (arriving_process && arriving_process->priority > running_process->priority)) {
        if (running_process && running_process->time_left > 0) add_queue_priop(running_process, PRIOP_queue);
        // prozess mit höchster prio aus queue
        running_process = queue_poll(PRIOP_queue);
    }
    return running_process;
}

// räumt speicher frei
void PRIOP_finish() {
    free_queue(PRIOP_queue);
}
