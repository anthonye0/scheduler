#include "../lib/queue.h"
#include <stdlib.h>
#include <stdio.h>

int queue_add(void *item, queue_object *queue) {
    if (!queue) return 1;

    queue_object *element = (queue_object *)malloc(sizeof(queue_object));
    if (!element) return 1;

    element->object = item;
    element->next = NULL;

    queue_object *ptr = queue;
    while (ptr->next != NULL) ptr = ptr->next;

    ptr->next = element;

    return 0;
}

void *queue_poll(queue_object *queue) {
    if (!queue || !queue->next) return NULL;

    queue_object *first = queue->next;
    void *item = first->object;

    queue->next = first->next;
    free(first);

    return item;
}

queue_object *new_queue() {
    queue_object *queue = (queue_object *)malloc(sizeof(queue_object));
    if (!queue) return NULL;

    queue->object = NULL;
    queue->next = NULL;

    return queue;
}

void free_queue(queue_object *queue) {
    if (!queue) return;

    queue_object *current = queue;
    while (current) {
        queue_object *next = current->next;
        free(current);
        current = next;
    }
}

void *queue_peek(queue_object *queue) {
    if (!queue || !queue->next) return NULL;

    return queue->next->object;
}