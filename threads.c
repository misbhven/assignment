/*
*File: threads.c
*Author: Dylan travers
*ID: 19128043
*Program: lift_sim_A
*
*This file holds functions that init threads, run producer/conusmer threads,
*and destroys all at the end.
*******************************************************************************/

#include "threads.h"

/* mutex lock */
pthread_mutex_t bufferMutex;

/* semaphores */
pthread_cond_t fullCond, emptyCond;

/* buffer */
buffer_item *buffer;

/* totals */
totals_item *consumerTotals;

/* flag for finishing threads, 1 = true, 0 = false */
int flag = 1;

/* producer runner method for threads */
int isempty()
{
    if (isEmpty(buffer->bufferQ))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

int isfull()
{
    if(buffer->bufferQ->size == buffer->bufferQ->limit)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
void* lift_R(void *param)
{
    Queue *allRequests = (Queue *)param;
    node *_node;

    while (flag == 1)
    {
        /* remove a request from the total requests */
        _node = dequeue(allRequests);

        /* wait condition whilst buffer is full */
        while (isfull())
        {
            pthread_cond_wait(&fullCond, &bufferMutex);
        }
        pthread_mutex_lock(&bufferMutex);

        /* start critical section */
        /* insert request into the buffer */
        enqueue(buffer->bufferQ, _node);
        /* increment the total requests counter */
        consumerTotals->requestsPreConsumption++;
        printf("---\nProduced: floor %d to floor %d.\nRequest No. ---\n", _node->data.source, _node->data.destination);
        /* exit critical section */


        pthread_mutex_unlock(&bufferMutex);
        pthread_cond_signal(&emptyCond);

    }
    /* shouldnt be reached */
    return NULL;
}

void* lift_consumer(void *in_lift_item)
{
    lift_item *lift = (lift_item *) in_lift_item;
    node *_node;
    int movement;

    while (flag == 1)
    {
        while (isEmpty(buffer->bufferQ))
        {
            pthread_cond_wait(&emptyCond, &bufferMutex);
        }
        pthread_mutex_lock(&bufferMutex);

        _node = dequeue(buffer->bufferQ);
        lift->requestTotal++;
        movement = abs((lift->prevPos - _node->data.source) + (_node->data.source - _node->data.destination));
        lift->moveTotal += movement;
        consumerTotals->moves += movement;
        consumerTotals->requests++;
        displayInfo(_node, lift, movement);
        /*
        printf("---\nLift %c consumed: floor %d to floor %d\nPrevious Floor: %d\nDetailed operation:\n\tGo from floor %d to floor %d\n\tGo from floor %d to floor %d\n\t#movement for this request: %d\n\t#Total movement: %d\n\t#request: %d\nCurrent Floor: %d.\n---\n", lift->liftName, _node->data.source, _node->data.destination, lift->prevPos, _node->data.source, _node->data.destination, lift->prevPos, _node->data.source, _node->data.source, _node->data.destination, movement, lift->requestTotal, _node->data.destination);
        */
        lift->prevPos = _node->data.destination;


        pthread_mutex_unlock(&bufferMutex);
        pthread_cond_signal(&fullCond);
        sleep(lift->sleepTime);
    }
    return NULL;
}


void threadInit(Queue* allRequests, int bufferSize, int sleepTime)
{
    Queue *bufferQ = NULL;
    pthread_t liftAID, liftBID, liftCID, liftRID;
    lift_item liftAItem, liftBItem, liftCItem;
    pthread_cond_init(&emptyCond, NULL);
    pthread_cond_init(&fullCond, NULL);


    /*if (sem_init(&empty, 0, bufferSize) !=0){
        fprintf(stderr, "could not init empty semaphore\n");
    }*/
    if(pthread_mutex_init(&bufferMutex, NULL) != 0)
    {
        fprintf(stderr, "could not init buffer mutex\n");
    }
    /*if(sem_init(&full, 0,0) != 0)
    {
        fprintf(stderr, "could not init full semaphore\n");

    }*/

    /* initialise the buffer struct */
    initBuffer(bufferQ, bufferSize);

    /* initialise each lift with its own struct */
    initLifts(&liftAItem, 'A', sleepTime);
    initLifts(&liftBItem, 'B', sleepTime);
    initLifts(&liftCItem, 'C', sleepTime);

    /* initialise total requests struct */
    initTotals(allRequests);

    /* create all threads for lift request producer and the three lift consumers */
    pthread_create(&liftRID, NULL, lift_R, allRequests);
    pthread_create(&liftAID, NULL, lift_consumer, &liftAItem);
    pthread_create(&liftBID, NULL, lift_consumer, &liftBItem);
    pthread_create(&liftCID, NULL, lift_consumer, &liftCItem);

    sleep(10);
    flag = 0;


    /* join all and destroy everything */
    pthread_join(liftRID, NULL);
    pthread_join(liftAID, NULL);
    pthread_join(liftBID, NULL);
    pthread_join(liftCID, NULL);
    pthread_mutex_destroy(&bufferMutex);
    pthread_cond_destroy(&fullCond);
    pthread_cond_destroy(&emptyCond);
    /*
    sem_destroy(&empty);
    sem_destroy(&full);
    */

}

void initBuffer(Queue *queue, int _bufferSize)
{
    /* initialise buffer */
    queue = initQueue(_bufferSize);
    if(queue == NULL)
    {
        fprintf(stderr, "could not init buffer queue.\n");
    }
    buffer->bufferSize = _bufferSize;
    buffer->bufferQ = queue;
}

void initLifts(lift_item *lift, char _liftName, int _sleepTime)
{
    /* initialise struct fields */

    lift->liftName = _liftName;
    lift->sleepTime = _sleepTime;
    lift->prevPos = 1;
    lift->currentPos = 1;
    lift->moveTotal = 0;
    lift->requestTotal = 0;
}

void initTotals(Queue *requestQ)
{
    consumerTotals->requests = requestQ->limit;
    consumerTotals->moves = 0;
    consumerTotals->requestsPreConsumption = 0;
}

void displayInfo(node *_node, lift_item *lift, int movement)
{
    int src, dest, prev;
    src = _node->data.source;
    dest = _node->data.destination;
    prev = lift->prevPos;

    printf("---\nLift %c consumed: floor %d to floor %d\n", lift->liftName, src, dest);
    printf("Previous Floor: %d\n", prev);
    printf("Detailed operation:\n\tGo from floor %d to floor %d\n\t", prev, src);
    printf("Go from floor %d to floor %d\n\t", src, dest);
    printf("#movement for this request: %d\n\t", movement);
    printf("#Total movement: %d\n\t", consumerTotals->moves);
    printf("#Total requests: %d\n", consumerTotals->requests);
    printf("Current Floor: %d", dest);
}
