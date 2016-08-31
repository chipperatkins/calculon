#include <stdio.h>
#include <stdlib.h>
#include "value.h"
#include "node.h"
#include "stack.h"

stack *newStack()
{
    stack *s = (stack *) malloc(sizeof(stack));
    if (s == 0) { fprintf(stderr,"out of memory"); exit(-1); }
    s->top = 0;
    s->top1 = 0;
    return s;
}

void push (stack *s,node *n)
{
    if (s->top == 0)
    {
        node *temp;
        temp = newValueNode(0,0);
        temp->next = 0;
        temp->value = n->value;
        s->top = temp;
    }
    else
    {
        node *temp;
        temp = newValueNode(0,0);
        temp->next = s->top;
        temp->value = n->value;
        s->top = temp;
    }
}

node *pop(stack *s)
{
    s->top1 = s->top;

    if (s->top1 == 0)
    {
        printf("\n Error : Trying to pop from empty stack");
        return 0;
    }
    else
        s->top = s->top->next;
    return s->top1;
}

void destroy(stack *s)
{
    s->top1 = s->top;

    while (s->top1 != 0)
    {
        s->top1 = s->top->next;
        free(s->top);
        s->top = s->top1;
        s->top1 = s->top1->next;
    }
    free(s->top1);
    s->top = 0;
}
