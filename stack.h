/***
 * stack header file
 * Author: Chipper Atkins
 */
typedef struct stack
{
    struct nodeobject *top;
    struct nodeobject *top1; //temp top for popping
} stack;

extern stack *newStack();
extern void push(stack *s,node *n);
extern node* pop(stack *s);