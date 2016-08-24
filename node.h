typedef struct nodeobject
    {
    value *value;
    char* key;
    struct nodeobject *next;
    struct nodeobject *left;
    struct nodeobject *right;
    } node;

extern node *newValueNode(value *v,node *next);
extern node *newBstNode(value *v, char* key, node *left, node *right);
extern char* getKey(node *n);