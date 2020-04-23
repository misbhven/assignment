#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "queue.h"

#define MAX_LINE_LENGTH 8
Queue *inFileInit(Queue *requests);

int getLines(FILE* file);

#endif
