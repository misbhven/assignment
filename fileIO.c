/*
File: fileIO.c
Author: Dylan Travers
ID: 19128043
Program: lift_sim

Purpose: to handle all file io between other functions and sim_in and sim_out
         files and their respective values
*******************************************************************************/

#include "fileIO.h"

Queue *inFileInit(Queue *requests)
{
    /* file pointer for sim_in.txt */
    FILE* inFile;
    /* each individual line that is read in */
    char line[MAX_LINE_LENGTH];
    char* inFileName = "sim_input.txt";
    int lines, i, src, dest;

    if((inFile = fopen(inFileName, "r")) != NULL)
    {
        /* get lines in sim_in.txt */
        lines = getLines(inFile);
        rewind(inFile);
        requests = initQueue(lines);
        if(requests == NULL)
        {
            fprintf(stderr, "Could not init request queue.\n");
        }
        /* iterate through lines */
        if(lines >= 50 && lines <= 100)
        {

            for(i = 0; i < lines; i++)
            {
                if(fgets(line, MAX_LINE_LENGTH, inFile) != NULL)
                {
                    /* get instructions from line */
                    sscanf(line, "%d %d", &src, &dest);
                    /* make sure instructions are within the floors 1-20 */
                    if(src >= 1 && src <= 20)
                    {
                        if(dest >= 1 && dest <= 20)
                        {
                            enqueue(requests, src, dest);
                            /* printf("Linked List head: %d %d\n", testData[0], testData[1]); */
                        }
                        else
                        {
                            printf("ERROR: Found incorrect input in %s, %d is an invalid floor. Line: %d\n", inFileName, dest, i+1);
                        }
                    }
                    else
                    {
                        printf("ERROR: Found incorrect input in %s, %d is an invalid floor. Line: %d\n", inFileName, src, i+1);
                    }
                }
                else
                {
                    perror("ERROR: sim_input.txt incorrectly formatted\n");
                }
            }
            fclose(inFile);
        }
        else
        {
            perror("ERROR: sim_input.txt is not working\n");
        }
    }
    else
    {
        perror("ERROR: Lift requests in sim_input.txt is out side of range 50 <= number of requests <= 100\n");
    }
    return requests;
}

int getLines(FILE* file)
{
    char ch;
    int lines;
    lines = 0;

    for(ch = fgetc(file); !feof(file); ch = fgetc(file))
    {
        if(ch == '\n')
        {
            lines++;
        }
    }
    return lines;
}
