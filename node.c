#include <stdio.h>
#include <stdlib.h>
#include "value.h"
#include "node.h"


static node *newNode(void);

/*****
 * Node Class
 * Chipper Atkins
 */

/*************** public interface *************/

node *newValueNode(value *v,node *n) { //creates a value node
    node *p = newNode();
    p->value = v;
    p->next = n;
    return p;
}

node *newBstNode(value *v, char* key, node *l, node *r) { //node for BST containing left and right for bulding trees
    node *n= newNode();
    n->value = v;
    n->key = key;
    n->left = l;
    n->right = r;
    return n;
}

/*************** private methods *************/

static node *newNode() { //allocates space for node
    node *n = (node *) malloc(sizeof(node));
    if (n == 0) { fprintf(stderr,"out of memory"); exit(-1); }
    return n;
}


