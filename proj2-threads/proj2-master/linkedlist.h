#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mythreads.h"
typedef struct node{
  int idNum;
  ucontext_t threadContext;
  struct node *next;
} node_t;
node_t * insert(node_t *head, int num);
node_t * insertStart(node_t* head, int num);
node_t * delNode(node_t *head);
node_t * rotate(node_t *head);
