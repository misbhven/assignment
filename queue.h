#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0


typedef struct Data {
    int source;
    int destination;
} requestData;

typedef struct Node {
    requestData data;
    struct Node *prev;
} node;

typedef struct Queue {
    node *head;
    node *tail;
    int size;
    int limit;
} Queue;

Queue *initQueue(int limit);
void destroyQueue(Queue *queue);
int enqueue(Queue *queue, node *item);
node * dequeue(Queue *queue);
int isEmpty(Queue* queue);

#endif
