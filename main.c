/*
File: main.c
Author: Dylan Travers
ID: 19128043
Program: lift_sim

Purpose: runs the main method
*******************************************************************************/
#include "main.h"

int main(int argc, char **argv)
{
    printf("\n\n\n----------\n* lift_sim_A (pthreads)\n* Starting...\n----------\n\n\n");
    /* cmd line arguements for buffer size and sleep time */
    int m, t;
    Queue *requests = NULL;

    /* check that number of arguements == 3, otherwise provide usage info */
    if(argc == 3)
    {
        requests = inFileInit(requests);
    }
    /* usage information for running ./program */
    else
    {
        printf("Usage: %s <buffer size> <time for elevator to sleep", argv[0]);
        exit(-1);
    }

    /* get cmd line args */
    m = atoi(argv[1]);
    t = atoi(argv[2]);


    printf("----------\n* Running threads...\n----------\n\n\n");

    threadInit(requests, m, t);

    printf("----------\n*lift_sim_A (pthreads)\n* Done!\n* Check sim_out.txt for results.\n----------\n\n\n");


    /* destroy request queue and all it's nodes */
    destroyQueue(requests);
}
