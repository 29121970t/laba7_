#include <stdlib.h>
#include <stdio.h>

typedef struct QueueElement {
    struct QueueElement* next;
    char data;
} QueueElement;

typedef struct Queue {
    size_t length;
    size_t maxLength;
    QueueElement* first;
    QueueElement* last;
} Queue;


Queue* createQueue(size_t maxLength) {
    Queue* res = malloc(sizeof(Queue));
    res->first = NULL;
    res->last = NULL;
    res->length = 0;
    res->maxLength = maxLength;
    return res;
}

int enQueue(Queue* queue, long data) {
    if (queue->length >= queue->maxLength) return 1;
    QueueElement* container = malloc(sizeof(QueueElement));
    container->data = data;
    container->next = NULL;
    if (queue->last) queue->last->next = container;
    if (queue->length == 0) queue->first = container;
    queue->last = container;
    queue->length++;
    return 0;
}
char deQueue(Queue* queue) {
    QueueElement* temp = queue->first;
    char data = temp->data;
    queue->first = temp->next;
    queue->length--;
    free(temp);
    return data;
}
int isQueueEmpty(Queue* queue) {
    return !queue->length;
}
int isQueueFull(Queue* queue) {
    return queue->length >= queue->maxLength;
}
size_t getQueueLength(Queue* queue) {
    return queue->length;
}
char peekFrontQueue(Queue * queue){
    return queue->first->data;
}
char peekBackQueue(Queue * queue){
    return queue->last->data;
}
void freeQueue(Queue* queue) {
    QueueElement* el2, * el1 = queue->first;
    while (el1) {
        el2 = el1->next;
        free(el1);
        el1 = el2;
    }
    free(queue);
}
void printQueue(Queue* queue) {
    if (isQueueEmpty(queue)) { printf("Queue is empty\n"); return; }
    QueueElement* el = queue->first;
    while (el) {
        printf("%c->", el->data);
        el = el->next;
    }
    puts("");
}
