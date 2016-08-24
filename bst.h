typedef struct bst
{
    struct nodeobject *root;
}bst;

extern bst *newBst();
extern node *insert(node *n, node *d);
extern node *deleteNode(node *root, char *key);
extern node *findMin(node *n);
extern node *findMax(node *n);
extern node *search(node *root, char *key);
extern void printInOrder(node *root);