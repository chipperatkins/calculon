/****
 * queue source file
 * Author: Chipper Atkins
 ****/
#include <stdio.h>
#include <stdlib.h>
#include "value.h"
#include "node.h"
#include "queue.h"

static queue *makeSpace(void);

/****** Public Interface ******/

queue *newQ() {
    queue *q = makeSpace();
    q->front = 0;
    q->rear = 0;
    return q;
}

void enQ(queue *q,node *n) { //enqueues node
    if(q->front == 0 && q->rear == 0) {
        q->front = q->rear = n;
        return;
    }
    q->rear->next = n;
    q->rear = n;
}

void enQQ(queue *q, queue *m) { //enqueues another queue
    if (q->front == 0 && q->rear == 0) {
        q->front  = m->front;
        q->rear = m->rear;
        return;
    }
    q->rear->next = m->front;
    q->rear = m->rear;
}

node* deQ(queue *q) { //dequeues node
    node *temp = q->front;
    if(q->front == 0) {
        printf("Queue is Empty\n");
        return temp;
    }
    if(q->front == q->rear)
        q->front = q->rear = 0;
    else {
        q->front = q->front->next;
    }
    return temp;
}

/***** Private Methods *****/

static queue *makeSpace() { //allocates space for queue
    queue *q = (queue *) malloc(sizeof(queue));
    if (q == 0) { fprintf(stderr,"out of memory"); exit(-1); }
    return q;
}