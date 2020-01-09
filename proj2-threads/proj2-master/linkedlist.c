#include "linkedlist.h"
node_t * insert(node_t * head,int num){
  node_t *temp;
  node_t *copy;
  temp = malloc(sizeof(node_t));
  temp->idNum = num;
  temp->next = NULL;
  if(head == NULL){
    head = temp;
  }else{
    copy = head;
    while(copy->next != NULL){
      copy = copy->next;
    }
    copy->next = temp;
  }
 return head;
}
node_t * insertStart(node_t* head, int num){
  node_t * temp;
  node_t * copy;
  temp = malloc(sizeof(node_t));
  temp->idNum = num;
  if(head == NULL){
    temp->next = NULL;
    head = temp;
  } else{
      copy = head;
      temp->next = copy;
      head = temp;
  }
  return head;
}
node_t * delNode(node_t * head){
  node_t *temp;
  temp = head;
  if(head->next != NULL)
  {
    head = head->next;
  }else{
    head = NULL;
  }
  free(temp);
  return head;
}
node_t * rotate(node_t *head){
  //node_t *temp;
  if(head == NULL){
    return head;
  } else if(head->next == NULL){
    return head;
  }else{
    head = insert(head, head->idNum);
    head = delNode(head);
    return head;
  }
}
