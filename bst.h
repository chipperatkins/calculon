typedef struct bst
{
    struct nodeobject *root;
}bst;

extern bst *newBst();
extern void insert(bst *b, node *n);
extern node *deleteNode(bst *b, node *n);