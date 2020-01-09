//Andrew Soby asoby C28319723 Cpsc 3220 Project 1 memory_shim.c
#define _GNU_SOURCE
void __attribute__ ((constructor)) init(void);
void __attribute__ ((destructor)) cleanup(void);

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>

//void *(* original_malloc ) ( size_t size );

// implement these functions
//void free ( void * ptr );
//void * malloc ( size_t size );
// how to call the original version of malloc
//void *(* original_malloc ) ( size_t size );
//original_malloc = dlsym ( RTLD_NEXT , ” malloc ”);
//void * ptr = original_malloc (17) ;

typedef struct node{
  int mallocSize;
  int hasBeenFreed; //0 if not yet freed 1 if freed
  struct node *next;
} node_t;

void *(*original_malloc)(size_t size) = NULL;
void (*original_free)(void *ptr) = NULL;

node_t * head = NULL; //original_malloc(sizeof(node_t));

//if I go the linked list route would me calling original_malloc work here?

void init(void){
    if(original_malloc == NULL)
    {
      original_malloc = dlsym(RTLD_NEXT, "malloc");
    }
    if(original_free == NULL)
    {
      original_free = dlsym(RTLD_NEXT, "free");
    }
    head = original_malloc(sizeof(node_t));
}

void * malloc(size_t size){
  //do work to track mallocs and size
  node_t *current = head;
  current->mallocSize = size;
  current->hasBeenFreed = 0;
  current->next = original_malloc(sizeof(node_t));
  current = current->next;

  void * ptr = original_malloc(size);

  return ptr;
}

void free(void * ptr){
  //find where this ptr was tracked
  node_t * currentFree = head;
  while(currentFree->next != NULL)
  {
    if(currentFree->hasBeenFreed == 0 && currentFree->mallocSize == sizeof(*ptr)){
        currentFree->hasBeenFreed = 1;
        break;
    }
  }
  original_free(ptr);
}

void cleanup(void){
  //clean up anything else
  node_t *print = head;
  int leakCount = 0;
  int totalLeaked = 0;
  while(print->next != NULL)
  {
    if(print->hasBeenFreed == 0){
      fprintf(stderr,"Leak   %d\n", print->mallocSize);
      leakCount++;
      totalLeaked += print->mallocSize;
    }
  }
  fprintf(stderr, "Total %d %d\n", leakCount, totalLeaked);
}
