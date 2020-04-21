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
    node *nd;
    while (!isEmpty(queue))
    {
        nd = dequeue(queue);
        free(nd);
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
int enqueue(Queue *queue, node *item)
{
    if ((queue == NULL) || (item == NULL))
    {
        /* bad data passed */
        return FALSE;
    }
    if (queue->size >= queue->limit)
    {
        /* queue full */
        return FALSE;
    }

    item->prev = NULL;
    /* queue empty */
    if (queue->size == 0)
    {
        queue->head = item;
        queue->tail = item;
    }
    else
    {
        queue->tail->prev = item;
        queue->tail = item;
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
node * dequeue(Queue *queue)
{
    node *item;
    if (isEmpty(queue))
    {
        return NULL;
    }
    item = queue->head;
    queue->head = (queue->head)->prev;
    queue->size--;
    return item;
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
    else
    {
        return FALSE;
    }
}

/*
Test method
int main(int argc, char* argv[])
{
    int i;
    int limit;
    Queue *q;
    node *_node;

    limit = atoi(argv[1]);
    q = initQueue(limit);


    for (i = 0; i < limit; i++)
    {
        _node = (node*) malloc(sizeof (node));
        _node->data.source = 1 + i;
        _node->data.destination = 5 + i;
        enqueue(q, _node);
    }

    while (!isEmpty(q))
    {
        _node = dequeue(q);
        printf("Dequeued: %d %d\n", _node->data.source, _node->data.destination);
        free(_node);
    }
    destroyQueue(q);
    return(EXIT_SUCCESS);
}
*/
