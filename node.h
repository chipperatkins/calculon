typedef struct nodeobject
    {
    value *value;
    char* key;
    struct nodeobject *next;
    struct nodeobject *left;
    struct nodeobject *right;
    struct nodeobject *parent;
    } node;

extern node *newValueNode(value *v,node *next);
extern node *newBstNode(value *v, char* key, node *left, node *right, node* parent);
extern char* getKey(node *n);