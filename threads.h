#ifndef THREADS_H
#define THREADS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "queue.h"

typedef struct _totals_item {
    int moves;
    int requests;
    int allRequests;
} totals_item;

typedef struct _buffer_item {
    int bufferSize;
    Queue *bufferQ;
    int sleepTime;
} buffer_item;

typedef struct _lift_item {
    char liftName;
    int moveTotal;
    int requestTotal;
    int prev;
} lift_item;


void* lift_R(void *param);

void* lift_consumer(void *in_lift_item);

void threadInit(Queue* allRequests, int bufferSize, int sleepTime);

void initBuffer(int _bufferSize, int _sleepTime);

void initLifts(lift_item *lift, char _liftName);

void initTotals(int size);

void displayInfo(int src, int dest, int prev, int movement, char name);

void consumerLogger(int src, int dest, int movement, int prev, char name);

void totalLogger(int requests, int moves);

void producerLogger(int src, int dest, int totalRequests);

#endif
