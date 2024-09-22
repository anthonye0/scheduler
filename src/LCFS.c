#include "../lib/LCFS.h"

static queue_object *LCFS_queue;

// Startup
int LCFS_startup() {
    LCFS_queue = new_queue(); 
    return (LCFS_queue == NULL) ? 1 : 0; 
}

process *LCFS_tick(process *running_process) {
    // überprüft ob Prozess gewechselt werden muss
    if (!running_process || !running_process->time_left) {
        running_process = queue_poll(LCFS_queue); //holt nächsten Prozess
    }
    // verbleibende zeit verringern
    if (running_process) {
        running_process->time_left--;
    }
    return running_process;
}

// neuer Prozess kommt
process *LCFS_new_arrival(process *arriving_process, process *running_process) {
    if (arriving_process) {
        queue_object *new_node = malloc(sizeof(queue_object));
        if (new_node) {
            // neuer Prozess direkt an Kopf der Schlange hinzugefügt wegen LCFS
            new_node->object = arriving_process;
            new_node->next = LCFS_queue->next;
            LCFS_queue->next = new_node;
        }
    }
    // laufenden Prozess ausgeben
    return running_process;
}

// Beendet scheduling und gibt Speicher frei
void LCFS_finish() {
    free_queue(LCFS_queue); 
}
