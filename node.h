typedef struct nodeobject
    {
    struct value value;
    struct nodeobject *next;
    struct nodeobject *left;
    struct nodeobject *right;
    struct nodeobject *parent;
    } node;

extern node *newValueNode(struct value v,node *next);
extern node *newBstNode(struct value v, node *left, node *right, node* parent);