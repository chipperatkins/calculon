#include <stdio.h>
#include "value.h"
#include "scanner.h"
#include "node.h"
#include "queue.h"
#include "stack.h"
#include "bst.h"

int main(int argc, char *argv[])
{
    node *n;
    value *v;
    v = newIntegerValue(8);
    n = newBstNode(v,"B",0,0);

    node *root = 0;
    root = insert(root,n);
    n = newBstNode(v,"C",0,0);
    root = insert(root,n);
    n = newBstNode(v,"A",0,0);
    root = insert(root,n);
    n = newBstNode(v,"@",0,0);
    insert(root,n);
}