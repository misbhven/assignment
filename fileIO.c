/*
File: fileIO.c
Author: Dylan Travers
ID: 19128043
Program: lift_sim

Purpose: to handle all file io between other functions and sim_in and sim_out
         files and their respective values
*******************************************************************************/



#include "fileIO.h"

void inFileInit()
{
    /* file pointer for sim_in.txt */
    FILE* inFile;
    /* each individual line that is read in */
    char line[MAX_LINE_LENGTH];
    char* inFileName = "sim_input.txt\n";
    int lines, src, dest, i, ii;
    int instruction[2];

    if((inFile = fopen(inFileName, "r")) != NULL)
    {
        /* get lines in sim_in.txt */
        lines = getLines(inFile);
        rewind(inFile);
        /* iterate through lines */
        for(i = 0; i < lines; i++)
        {
            if(fgets(line, MAX_LINE_LENGTH, inFile) != NULL)
            {
                /* get instructions from line */
                sscanf(line, "%d %d", &src, &dest);
                /* make sure instructions are within the floors 1-20 */
                if(src >= 1 || src <= 20)
                {
                    if(dest >= 1 || dest <= 20)
                    {
                        instruction[0] = src;
                        instruction[1] = dest;
                        printf("%d %d\n", instruction[0], instruction[1]);
                    }
                    else
                    {
                        printf("ERROR: Found incorrect input in %s, %d is an invalid floor. Line: %d\n", inFileName, dest, i);
                    }
                }
                else
                {
                    printf("ERROR: Found incorrect input in %s, %d is an invalid floor. Line: %d\n", inFileName, src, i);
                }
            }
            else
            {
                perror("ERROR: map file incorrectly formatted\nIncorrect line = %d %s\n", i, line);
            }
        }
        fclose(inFile);
    }
    else
    {
        perror("ERROR: %s is not working\n", inFileName);
    }

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
