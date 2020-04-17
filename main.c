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
    /* cmd line arguements for buffer size and sleep time */
    int m, t;


    /* check that number of arguements == 3, otherwise provide usage info */
    if(argc == 3)
    {
        inFileInit();
    }
    /* usage information for running ./program */
    else
    {
        printf("Usage: %s <buffer size> <time for elevator to sleep", argv[0]);
        exit(-1);
    }

    /* get cmd line args */
    int m = atoi(argv[1]);
    int t = atoi(argv[2]);
}
