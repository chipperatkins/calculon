#include <stdio.h>
#include <stdlib.h>
#include "value.h"
#include "node.h"
#include "queue.h"

static queue *makeSpace(void);

queue *newQ()
{
    queue *q = makeSpace();
    q->front = 0;
    q->rear = 0;
    return q;
}

void enQ(queue *q,node *n)
{
    if(q->front == 0 && q->rear == 0)
    {
        q->front = q->rear = n;
        return;
    }
    q->rear->next = n;
    q->rear = n;
}

void enQQ(queue *q, queue *m)
{
    if (q->front == 0 && q->rear == 0)
    {
        q->front  = m->front;
        q->rear = m->rear;
        return;
    }
    q->rear->next = m->front;
    q->rear = m->rear;
}

node* deQ(queue *q)
{
    node *temp = q->front;
    if(q->front == 0)
    {
        printf("Queue is Empty\n");
        return temp;
    }
    if(q->front == q->rear)
    {
        q->front = q->rear = 0;
    }
    else
    {
        q->front = q->front->next;
    }
    return temp;
}

static queue *makeSpace()
{
    queue *q = (queue *) malloc(sizeof(queue));
    if (q == 0) { fprintf(stderr,"out of memory"); exit(-1); }
    return q;
}