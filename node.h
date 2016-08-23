typedef struct nodeobject
    {
    value *value;
    struct nodeobject *next;
    struct nodeobject *left;
    struct nodeobject *right;
    struct nodeobject *parent;
    } node;

extern node *newValueNode(value *v,node *next);
extern node *newBstNode(value *v, node *left, node *right, node* parent);