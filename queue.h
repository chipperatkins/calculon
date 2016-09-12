/****
 * queue header file
 * Author: Chipper Atkins
 ****/
typedef struct queue
{
    struct nodeobject *front;
    struct nodeobject *rear;
} queue;

extern queue *newQ();
extern void enQ(queue *q,node *n);
extern void enQQ(queue *q, queue *m); //for merging queues
extern node* deQ(queue *q);