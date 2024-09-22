#include "../lib/RR.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * RR ist als Beispiel vorgegeben
 */

static queue_object *RR_queue;
static int q; // quantum für jeden prozess
static int tp; // zeit die prozess schon ausgeführt wurde

process *RR_tick(process *running_process) {
    if (!running_process || running_process->time_left == 0 || tp >= q) {
        tp = 0; 
        if (running_process && running_process->time_left > 0) {
            queue_add(running_process, RR_queue); // füg den aktuellen Prozess wieder zu queue hinzu falls nicht ausgeführt
        }
        running_process = queue_poll(RR_queue); // holt nächsten prozess
    }
    if (running_process) {
        tp++; // zeit die für den aktuellen prozess verbraucht wurde 1 hoch
        running_process->time_left--; //zeit die verbleibt um 1 runter
    }
    return running_process;
}

int RR_startup(int quantum) {
    q = quantum; // setzt quantum für prozesse
    tp = 0; // verbrauchte zeit zurücksetzen
    RR_queue = new_queue(); 
    return (RR_queue == NULL) ? 1 : 0; 
}

process *RR_new_arrival(process *arriving_process, process *running_process) {
    if (arriving_process) {
        queue_add(arriving_process, RR_queue); 
    }
    return running_process; 
}

void RR_finish() {
    free_queue(RR_queue); 
}
