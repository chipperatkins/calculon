#include <stdio.h>
#include <stdlib.h>
#include "value.h"
#include "node.h"
#include "bst.h"

//TODO allow duplicates

static void helper(bst *b,node *root, node *n);

bst *newBst()
{
    bst *b = (bst *) malloc(sizeof(bst));
    if (b == 0) { fprintf(stderr,"out of memory"); exit(-1); }
    b->root = 0;
    return b;
}

void insert(bst *b, node *n)
{
    if (b->root == 0)
        b->root = n;
    else
        helper(b,b->root,n);
}

static void helper(bst *b,node *current, node *n)
{
    current = b->root;
    if (n->value->rval < current->value->rval) //n is left
    {
        if (current->left == 0)
        {
            current->left = n;
        }
        else
            helper(b,current->left, n);
    }
    else //n id right
    {
        if (current->right == 0)
        {
            current->right = n;
        }
        else
            helper(b,current->right, n);
    }
}

node *deleteNode(bst *b, node *n)
{
    return 0;
}