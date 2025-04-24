#ifndef __INCLUDE_QUEUE_LIB__
#define __INCLUDE_QUEUE_LIB__
#include <stdlib.h>


typedef struct QueueElement {
    struct QueueElement* next;
    char data;
} QueueElement;

typedef struct DequeElement {
    struct DequeElement* next;
    char data;
} DequeElement;

QueueElement* createQueueElement(void);
char deQueue(QueueElement** queuePtr); 
int isQueueEmpty(QueueElement* queue);
QueueElement* enQueue(QueueElement* queue, char data);
size_t calcQueueLength(QueueElement* queue) ;
char peekBackQueue(QueueElement* queue);
char peekFrontQueue(QueueElement* queue);
void printQueue(QueueElement* queue);
void freeQueue(QueueElement* queue);
int isCharString(char* str, size_t len);
int areQueuesEqual(QueueElement** queue1ptr, QueueElement** queue2ptr);



DequeElement* createDequeElement(void);
char deDequeFront(DequeElement** queuePtr);
char deDequeBack(DequeElement** queuePtr);
int isDequeEmpty(DequeElement* queue);
DequeElement* enDequeBack(DequeElement* deque, char data);
DequeElement* enDequeFront(DequeElement* deque, char data);
size_t calcDequeLength(DequeElement* queue);
char peekBackDeque(DequeElement* queue);
char peekFrontDeque(DequeElement* queue);
void printDeque(DequeElement* queue) ;
void freeDeque(DequeElement* queue);

#endif