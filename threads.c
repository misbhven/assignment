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

/* conditions */
pthread_cond_t canProduce, canConsume;

/* buffer */
buffer_item *buffer;

/* totals */
totals_item *consumerTotals;

/* out file pointer */
int flag;

/* producer runner method for threads */
void* lift_R(void *param)
{
    Queue *allRequests = (Queue *)param;
    node *prodNode;
    int src, dest, totalRequests;
    totalRequests = 0;

    while (allRequests->size !=0)
    {

        /* lock mutex */
        /* wait condition whilst buffer is full */
        pthread_mutex_lock(&bufferMutex);
        while(buffer->bufferQ->size == buffer->bufferSize)
        {
            pthread_cond_wait(&canProduce, &bufferMutex);
        }

        /* remove a request from the total requests */
        prodNode = getRequest(allRequests);
        src = prodNode->source;
        dest = prodNode->destination;

        /* insert request into the buffer */
        enqueue(buffer->bufferQ, src, dest);
        /* increment the total requests counter */
        totalRequests++;
        producerLogger(src, dest, totalRequests);
        /* printf("---\nProduced: floor %d to floor %d.\nRequest Num: %d\n---\n", src, dest, totalRequests); */
        /* exit critical section */
		
        dequeue(allRequests);
		if (consumerTotals->requests < consumerTotals->allRequests)
		{
			flag = 1;
		}

        pthread_cond_signal(&canConsume);
        pthread_mutex_unlock(&bufferMutex);
    }
    printf("Produced finished\n");
    return NULL;
}

void* lift_consumer(void *in_lift_item)
{
    lift_item *lift = (lift_item *) in_lift_item;
    node *consNode;
    int src, dest, movement;
    /* only consume whilst there are items left to consume */
    while(flag != 1)
    {
        pthread_mutex_lock(&bufferMutex);
        /* 2nd statement prevents deadlock */
        while (buffer->bufferQ->size == 0 && consumerTotals->requests < consumerTotals->allRequests)
        {
            pthread_cond_wait(&canConsume, &bufferMutex);
        }

        /* check incase race condition occurs */
        if(flag != 1)
        {
            consNode = getRequest(buffer->bufferQ);
            src = consNode->source;
            dest = consNode->destination;
            movement = abs((lift->prev - src) + (src - dest));
            lift->requestTotal++;
            lift->moveTotal += movement;
            consumerTotals->moves += movement;
            consumerTotals->requests++;
            consumerLogger(src, dest, movement, lift->prev, lift->liftName);
            /* displayInfo(src, dest, lift->prev, movement, lift->liftName); */
            lift->prev = dest;
            dequeue(buffer->bufferQ);
        }

        pthread_cond_signal(&canProduce);
        pthread_mutex_unlock(&bufferMutex);
        sleep(buffer->sleepTime);
    }
    printf("\nLift %c thread ended\n", lift->liftName);
    return NULL;
}


void threadInit(Queue* allRequests, int bufferSize, int sleepTime)
{
	flag = 0;
    pthread_t liftAID, liftBID, liftCID, liftRID;
    lift_item liftAItem, liftBItem, liftCItem;

	initBuffer(bufferSize, sleepTime);
	initLifts(&liftAItem, 'A');
	initLifts(&liftBItem, 'B');
	initLifts(&liftCItem, 'C');
	initTotals(allRequests->size);


    pthread_cond_init(&canProduce, NULL);
    pthread_cond_init(&canConsume, NULL);

	
    if(pthread_mutex_init(&bufferMutex, NULL) != 0)
    {
        fprintf(stderr, "could not init buffer mutex\n");
    }

/*
    * initialise the buffer struct 
    initBuffer(bufferQ, bufferSize, sleepTime);

    * initialise each lift with its own struct 
    initLifts(&liftAItem, 'A');
    initLifts(&liftBItem, 'B');
    initLifts(&liftCItem, 'C');

    * initialise total requests struct 
    initTotals(allRequests->size);
*/
    /* create all threads for lift request producer and the three lift consumers */
    pthread_create(&liftRID, NULL, lift_R, allRequests);
    pthread_create(&liftAID, NULL, lift_consumer, (void*)&liftAItem);
    pthread_create(&liftBID, NULL, lift_consumer, (void*)&liftBItem);
    pthread_create(&liftCID, NULL, lift_consumer, (void*)&liftCItem);

    /* join all and destroy everything */
    pthread_join(liftRID, NULL);
    pthread_join(liftAID, NULL);
    pthread_join(liftBID, NULL);
    pthread_join(liftCID, NULL);
    printf("Got here\n");

    totalLogger(consumerTotals->requests, consumerTotals->moves);
    pthread_mutex_destroy(&bufferMutex);
    pthread_cond_destroy(&canConsume);
    pthread_cond_destroy(&canProduce);

    destroyQueue(buffer->bufferQ);
    free(consumerTotals);
    free(buffer);
}

void initBuffer(int _bufferSize, int _sleepTime)
{
    /* initialise buffer */
    buffer = (buffer_item*)malloc(sizeof(buffer_item));
    buffer->bufferSize = _bufferSize;
    buffer->bufferQ = initQueue(_bufferSize);
    buffer->sleepTime = _sleepTime;
}

void initLifts(lift_item *lift, char _liftName)
{
    /* initialise struct fields */

    lift->liftName = _liftName;
    lift->prev = 1;
    lift->moveTotal = 0;
    lift->requestTotal = 0;
}

void initTotals(int size)
{
    consumerTotals = (totals_item*)malloc(sizeof(totals_item));
    consumerTotals->requests = 0;
    consumerTotals->moves = 0;
    consumerTotals->allRequests = size;
}

void displayInfo(int src, int dest, int prev, int movement, char name)
{

    printf("---\nLift %c consumed: floor %d to floor %d\n", name, src, dest);
    printf("Previous Floor: %d\n", prev);
    printf("Detailed operation:\n\tGo from floor %d to floor %d\n\t", prev, src);
    printf("Go from floor %d to floor %d\n\t", src, dest);
    printf("#movement for this request: %d\n\t", movement);
    printf("#Total movement: %d\n\t", consumerTotals->moves);
    printf("#Total requests: %d\n", consumerTotals->requests);
    printf("Current Floor: %d\n", dest);
}

void consumerLogger(int src, int dest, int movement, int prev, char name)
{
    FILE* logfile;

    if((logfile = fopen("sim_out.txt", "a")) != NULL)
    {
        fprintf(logfile, "-------------------------------------------------------\n");
        fprintf(logfile, "Lift %c consumed: floor %d to floor %d\n", name, src, dest);
        fprintf(logfile, "Previous Floor: %d\n", prev);
        fprintf(logfile, "Detailed operation:\n\tGo from floor %d to floor %d\n\t", prev, src);
        fprintf(logfile, "Go from floor %d to floor %d\n\t", src, dest);
        fprintf(logfile, "#movement for this request: %d\n\t", movement);
        fprintf(logfile, "#Total movement: %d\n\t", consumerTotals->moves);
        fprintf(logfile, "#Total requests: %d\n", consumerTotals->requests);
        fprintf(logfile, "Current Floor: %d\n", dest);
    }
    else
    {
        perror("ERROR: couldnt open output file");
    }
    fclose(logfile);
}

void totalLogger(int requests, int moves)
{
    FILE* logfile;

    if((logfile = fopen("sim_out.txt", "a")) != NULL)
    {
        fprintf(logfile, "-------------------------------------------------------\n");
        fprintf(logfile, "Total Number of Requests: %d\nTotal movements: %d\n", requests, moves);
        fprintf(logfile, "-------------------------------------------------------\n");
    }
    else
    {
        perror("ERROR: couldnt open output file");
    }
    fclose(logfile);
}

void producerLogger(int src, int dest, int totalRequests)
{
    FILE* logfile;

    if((logfile = fopen("sim_out.txt", "a")) != NULL)
    {
        fprintf(logfile, "-------------------------------------------------------\n");
        fprintf(logfile, "Produced: floor %d to floor %d.\nRequest Num: %d\n", src, dest, totalRequests);
    }
    else
    {
        perror("ERROR: couldnt open output file");
    }
    fclose(logfile);
}
