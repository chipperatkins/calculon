typedef struct queue
{
    struct nodeobject *front;
    struct nodeobject *rear;
} queue;

extern queue *newQ();
extern void enQ(queue *q,node *n);
extern node* deQ(queue *q);