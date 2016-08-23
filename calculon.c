#include <stdio.h>
#include "value.h"
#include "scanner.h"
#include "node.h"
#include "queue.h"
#include "stack.h"

int main(int argc, char *argv[])
{
    value v = *newValueV("Q1A");

    node *n;

    stack *q1 = newStack();
    stack *q2 = newStack();

    n = newValueNode(v,0);
    push(q1,n);
    v.sval = "Q1B";
    n = newValueNode(v,0);
    push(q1,n);

    v.sval = "Q2A";
    n = newValueNode(v,0);
    push(q2,n);
    v.sval = "Q2B";
    n = newValueNode(v,0);
    push(q2,n);


    while (q1->top != 0 && q2->top != 0)
    {
        printf("%s %s\n",pop(q1)->value.sval,pop(q2)->value.sval);
    }

    v.rval = 88.90;

    printf("%s %f\n",v.sval,v.rval);
}