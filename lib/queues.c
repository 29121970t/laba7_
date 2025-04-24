#include <stdlib.h>
#include <stdio.h>

#include "parseLib5.h"

typedef struct QueueElement {
    struct QueueElement* next;
    char data;
} QueueElement;

QueueElement* createQueueElement(void) {
    QueueElement* el = malloc(sizeof(QueueElement));
    el->data = 0;
    el->next = NULL;
    return el;
}
char deQueue(QueueElement** queuePtr) {
    QueueElement* queue = *queuePtr;
    char toRet = queue->data;
    *queuePtr = queue->next;
    if(queue->next == NULL) *queuePtr = NULL;
    free(queue);
    return toRet;
}
int isQueueEmpty(QueueElement* queue) {
    return queue == NULL;
}
QueueElement* enQueue(QueueElement* queue, char data) {
    QueueElement* el = createQueueElement();
    QueueElement* ogQueue = queue;
    el->data = data;
    if (!queue) return el;

    while (queue->next) {
        queue = queue->next;
    }

    queue->next = el;
    return ogQueue;
}
size_t calcQueueLength(QueueElement* queue) {
    size_t counter = 0;
    if(!queue) return 0;
    while (queue->next) {
        counter++;
        queue = queue->next;
    }
    return counter;
}
char peekBackQueue(QueueElement* queue) {
    while (queue->next) {
        queue = queue->next;
    }
    return queue->data;
}
char peekFrontQueue(QueueElement* queue) {
    return queue->data;
}
void printQueue(QueueElement* queue) {
    if(isQueueEmpty(queue)) {printf("Queue is empty\n"); return;}
    while (queue->next) {
        printf("%c->", queue->data);
        queue = queue->next;
    }
    printf("%c\n", queue->data);
}
void freeQueue(QueueElement* queue) {
    if(queue == NULL) return;
    QueueElement* el = NULL;
    while (queue) {
        el = queue->next;
        free(queue);
        queue = el;
    }

}
int areQueuesEqual(QueueElement** queue1ptr, QueueElement** queue2ptr) {
    QueueElement* queue1 = *queue1ptr;
    QueueElement* queue2 = *queue2ptr;

    while (!isQueueEmpty(queue1) && !isQueueEmpty(queue2)) {

        if (deQueue(&queue1) != deQueue(&queue2)) return 0;
    }
    *queue1ptr  = queue1;
    *queue2ptr  = queue2;

    return isQueueEmpty(queue1) && isQueueEmpty(queue2);
    
}

typedef struct DequeElement {
    struct DequeElement* next;
    char data;
} DequeElement;

DequeElement* createDequeElement(void) {
    DequeElement* el = malloc(sizeof(DequeElement));
    el->data = 0;
    el->next = NULL;
    return el;
}
char deDequeFront(DequeElement** queuePtr) {
    DequeElement* temp = *queuePtr;
    char toRet = temp->data;
    *queuePtr = temp->next;
    free(temp);
    return toRet;
}
char deDequeBack(DequeElement** queuePtr) {
    DequeElement* el2, * el1 = *queuePtr;
    while (el1->next) {
        el2 = el1;
        el1 = el1->next;
    }
    char toRet = el1->data;
    free(el1);
    el2->next = NULL;
    return toRet;
}
int isDequeEmpty(DequeElement* queue) {
    return queue == NULL;
}
DequeElement* enDequeBack(DequeElement* deque, char data) {
    DequeElement* el = createDequeElement();
    DequeElement* ogQueue = deque;
    el->data = data;
    if (!deque) return el;

    while (deque->next) {
        deque = deque->next;
    }
    deque->next = el;
    return ogQueue;
}
DequeElement* enDequeFront(DequeElement* deque, char data) {
    DequeElement* el = createDequeElement();
    el->data = data;
    el->next = deque;
    return el;
}
size_t calcDequeLength(DequeElement* queue) {
    size_t counter = 0;
    if (!queue) return 0;
    while (queue->next) {
        counter++;
        queue = queue->next;
    }
    return counter;
}
char peekBackDeque(DequeElement* queue) {
    while (queue->next) {
        queue = queue->next;
    }
    return queue->data;
}
char peekFrontDeque(DequeElement* queue) {
    return queue->data;
}
void printDeque(DequeElement* queue) {
    while (queue->next) {
        printf("%c->", queue->data);
        queue = queue->next;
    }
    printf("%c\n", queue->data);
}
void freeDeque(DequeElement* queue) {
    DequeElement* el = NULL;
    while (queue->next) {
        el = queue->next;
        free(queue);
        queue = el;
    }
    free(queue);
}
