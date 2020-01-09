//Andrew Soby asoby C28319723 Cpsc 3220 Project 1 memory_shim.c
#define _GNU_SOURCE
void __attribute__ ((constructor)) init(void);
void __attribute__ ((destructor)) cleanup(void);

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>

//linked list structure to keep track of the poniter and size allocations
typedef struct node{
  int mallocSize;
  int hasBeenFreed; //0 if not yet freed 1 if freed
  void * ptr; //to make sure we are freeing the correct pointer
  struct node *next;
} node_t;

//will set these function pointers in constructor
void *(*original_malloc)(size_t size) = NULL;
void (*original_free)(void *ptr) = NULL;

node_t * head = NULL;
node_t *current = NULL;

//constructor sets the original free and malloc and creates our ll head
void init(void){
    if(original_malloc == NULL){
      original_malloc = dlsym(RTLD_NEXT, "malloc");
    }
    if(original_free == NULL){
      original_free = dlsym(RTLD_NEXT, "free");
    }
    head = original_malloc(sizeof(node_t));
    current = head;
}

void * malloc(size_t size){
  //place the malloc call into our linked list
  current->mallocSize = size;
  //set to 0 to show it hasn't been freed
  current->hasBeenFreed = 0;
  //call original malloc so no recursion
  current->next = original_malloc(sizeof(node_t));
  void * ptr = original_malloc(size);
  current->ptr = ptr;

  //increment linked list
  current = current->next;
  current->next = NULL;

  //return the pointer we malloced space for
  return ptr;
}

void free(void * ptr){
  //find where this ptr was tracked
  node_t * currentFree = head;

  //iterate ll to find the pointer to be freed
  while(currentFree->next != NULL){
    //check if it hadnt been freed and is the same pointer
    if(currentFree->hasBeenFreed == 0 && currentFree->ptr == ptr){
        //set to one to show it has been freed
        currentFree->hasBeenFreed = 1;
        break;
    }
    currentFree = currentFree->next;
  }
  //call original free however I could do this above in the if statement  
  original_free(ptr);
}

void cleanup(void){
  //clean up anything else
  node_t *print = head;
  //leak count trackers
  int leakCount = 0;
  int totalLeaked = 0;

  while(print->next != NULL){
    if(print->hasBeenFreed == 0){
      //print to stderr
      fprintf(stderr,"LEAK\t%d\n", print->mallocSize);
      leakCount++;
      totalLeaked += print->mallocSize;
    }
    print = print->next;
  }
  fprintf(stderr, "TOTAL\t%d\t%d\n", leakCount, totalLeaked);
}
