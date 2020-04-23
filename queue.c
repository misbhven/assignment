#include "queue.h"

/* init the queue, pass a limit to the queue, and it will allocate memory to
the queue, ensures that the limit is not 0 and sets it to 101. Returns a pointer
to a queue data structure. */
Queue *initQueue(int limit)
{
    Queue *queue = (Queue*) malloc(sizeof (Queue));
    if (queue == NULL)
    {
        return NULL;
    }
    if (limit <= 0)
    {
        limit = 101;
    }

    queue->limit = limit;
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

/* IMPORT: pointer to a Queue
   EXPORT: nothing
   PURPOSE: frees items within the queue and frees the queue. */

void destroyQueue(Queue *queue)
{
    while(queue->size != 0)
    {
        dequeue(queue);
    }
    free(queue);
}
/*
IMPORT: pointer to a queue and a pointer to a node item.
EXPORT: either true or false, depending if the enqueue was successful
PURPOSE: Add a provided node item to the queue of other node items, basic
         error checking is done to make sure that the item and queue is non NULL
         and that the queue hasn't exceeded it's node limit
*/
int enqueue(Queue *queue, int src, int dest)
{
    node *_node;

    if (queue == NULL)
    {
        /* bad data passed */
        printf("\n\nError: bad data passed to queue.\n\n");
        return FALSE;
    }
    if (queue->size >= queue->limit)
    {
        /* queue full */
        printf("\n\nError: tried to add data to full queue\n\n");
        return FALSE;

    }
    _node = (node*)malloc(sizeof(node));

    _node->source = src;
    _node->destination = dest;

    _node->prev = NULL;
    /* queue empty */
    if (queue->size == 0)
    {
        queue->head = _node;
        queue->tail = _node;
    }
    else
    {
        queue->tail->prev = _node;
        queue->tail = _node;
    }
    queue->size++;
    return TRUE;
}

/*
IMPORT: pointer to a queue
EXPORT: a node pointer to the dequeued object
PURPOSE: pop the first item off the queue and return it, basic error checking to
         make sure the queue isnt already empty. Makes the next item in line the
         new head.
*/
node *getRequest(Queue *queue)
{
    node *data;
    if (isEmpty(queue))
    {
        printf("ERROR: tried to get request on empty queue\n");
        return NULL;
    }
    else
    {
        data = queue->head;
        return data;
    }
}

void dequeue(Queue *queue)
{
    node* temp = queue->head;
    if(queue->head == NULL)
    {
        printf("ERROR: could not dequeue, queue is empty already...\n");
    }
    else
    {
        queue->head = temp->prev;
        queue->size--;
        free(temp);
    }
}

/*
IMPORT: pointer to a queue
EXPORT: either true or false, depending if the queue has items in it
PURPOSE: checks whether the queue has things in it.
*/
int isEmpty(Queue* queue)
{
    if (queue == NULL)
    {
        return FALSE;
    }
    if (queue->size == 0)
    {
        return TRUE;
    }
    if(queue->head == NULL)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
