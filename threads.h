#ifndef THREADS_H
#define THREADS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "queue.h"

typedef struct _totals_item {
    int moves;
    int requests;
    int requestsPreConsumption;
} totals_item;

typedef struct _buffer_item {
    int bufferSize;
    int bufferElements;
    Queue *bufferQ;
} buffer_item;

typedef struct _lift_item {
    int sleepTime;
    char liftName;
    int prevPos;
    int currentPos;
    int moveTotal;
    int requestTotal;
} lift_item;

int isempty();

int isfull();

void* lift_R(void *param);

void* lift_consumer(void *in_lift_item);

void threadInit(Queue* allRequests, int bufferSize, int sleepTime);

void initBuffer(Queue *queue, int _bufferSize);

void initLifts(lift_item *lift, char _liftName, int _sleepTime);

void initTotals(Queue* requestQ);

void displayInfo(node *_node, lift_item *lift, int movement);

#endif
