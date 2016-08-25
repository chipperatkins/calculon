#include <stdio.h>
#include <stdlib.h>
#include "value.h"
#include "node.h"
#include "bst.h"
#include "string.h"

//TODO allow duplicates
//TODO make objective

/***** Public Methods *****/

bst *newBst()
{
    bst *b = (bst *) malloc(sizeof(bst));
    if (b == 0) { fprintf(stderr,"out of memory"); exit(-1); }
    return b;
}

node *insert(node *n, node *d)
{
    if (n == 0)
    {
        node *t;
        t = d;
        return t;
    }
    if (strcmp(d->key,n->key)>0)
        n->right = insert(n->right,d);
    else if (strcmp(d->key,n->key)<0)
        n->left = insert(n->left,d);
    return n;
}

node *findMin(node *n)
{
    if (n == 0)
        return 0;
    if (n->left != 0)
        return findMin(n->left);
    else
        return n;
}

node *findMax(node *n)
{
    if (n == 0)
        return 0;
    if (n->right != 0)
        return findMax(n->right);
    else
        return n;
}

node *deleteNode(node *root, char *key)
{
    node *t;
    if(root == 0)
        return 0;
    else if(strcmp(key,root->key)<0)
        root->left = deleteNode(root->left,key);
    else if(strcmp(key,root->key)>0)
        root->right = deleteNode(root->right,key);
    else
    {
        if (root->right != 0 && root->left != 0)
        {
            t = findMin(root->right);
            root->value = t->value;
            root->key = t->key;
            root -> right = deleteNode(root->right,t->key);
        }
        else
        {
            t = root;
            if (root->left == 0)
                root = root->right;
            else if (root->right == 0)
                root = root->left;
            free(t);
        }
    }
    return root;
}

node *search(node *r, char *key)
{
    if (r == 0)
        return 0;
    if (strcmp(key,r->key)>0)
        return search(r->right,key);
    else if (strcmp(key,r->key)<0)
        return search(r->left,key);
    else
    {
        return r;
    }
}

void printInOrder(node *r)
{
    if (r==0)
        return;

    printf("\"%s\" ",r->key);
    printInOrder(r->left);
    printInOrder(r->right);
}