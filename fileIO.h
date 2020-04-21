#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "queue.h"

#define MAX_LINE_LENGTH 8
void inFileInit(Queue *requests, node *_node);

int getLines(FILE* file);

#endif
