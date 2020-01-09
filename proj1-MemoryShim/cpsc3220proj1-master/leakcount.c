//Andrew Soby asoby C28319723 Cpsc 3220 Project 1 leakcount.
#include <stdio.h>
#define _GNU_SOURCE
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "node.h"
//typedef struct node{
//  int val;
//  struct node *next;
//} node_t;

int main(int argc, char **argv){//, char **envp){
  node_t * head = original_malloc(sizeof(node_t));
  node_t * current = head;
  //add
  //current->val = i;
  //current->next = malloc(sizeof(node_t));
  //current = current->next;
  //current->next = NULL;
//  printf("%s\n", argv[1]);
  if(fork() == 0){
    //im in the child run execvp
    execvpe(argv[1], argv[1], NULL);
  }
  else{
    //Parents
  }
  while(head->next != NULL)
  {
    printf("%d", head->val);
    head = head->next;
  }

  return 0;
}
