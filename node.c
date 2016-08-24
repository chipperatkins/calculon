#include <stdio.h>
#include <stdlib.h>
#include "value.h"
#include "node.h"


static node *newNode(void);

/*************** public interface *************/

node *newValueNode(value *v,node *n)
    {
    node *p = newNode();
    p->value = v;
    p->next = n;
    return p;
    }

node *newBstNode(value *v, char* key, node *l, node *r)
{
    node *n= newNode();
    n->value = v;
    n->key = key;
    n->left = l;
    n->right = r;
    return n;
}

char* getKey(node* n)
{
    return n->key;
}
/*************** private methods *************/

static node *newNode()
    {
    node *n = (node *) malloc(sizeof(node));
    if (n == 0) { fprintf(stderr,"out of memory"); exit(-1); }
    return n;
    }


