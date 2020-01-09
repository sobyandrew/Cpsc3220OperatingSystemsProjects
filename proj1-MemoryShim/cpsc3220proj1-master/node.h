//node.h headerFile

typedef struct node{
  int val;
  struct node *next;
} node_t;

extern node_t *head;
extern node_t *current;
