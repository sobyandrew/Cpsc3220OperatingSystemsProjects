//Andrew Soby asoby C28319723 Cpsc 3220 Project 1 memory_shim.c
#define _GNU_SOURCE

//void __attribute__ ((constructor)) init(void);
//void __attribute__ ((destructor)) cleanup(void);

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include "node.h"
//node_t * head = ;
//void *(* original_malloc ) ( size_t size );

// implement these functions
//void free ( void * ptr );
//void * malloc ( size_t size );
// how to call the original version of malloc
//void *(* original_malloc ) ( size_t size );
//original_malloc = dlsym ( RTLD_NEXT , ” malloc ”);
//void * ptr = original_malloc (17) ;
//head = malloc(sizeof(node_t));
//node_t * current;

void *(* original_malloc ) ( size_t size ) = NULL;
void *(* original_free)(void *ptr) = NULL;

void * malloc(size_t size){
  current->val = size;
  current->next = original_malloc(sizeof(node_t));
  current = current->next;
  current->next = NULL;

  if(!original_malloc){
    original_malloc = dlsym(RTLD_NEXT, "malloc");
  }
  void * ptr = original_malloc(size);

  return ptr;
}

void free(void * ptr){
  //void *(* original_free)(void *) = NULL;
  if(!original_free){
    original_free = dlsym(RTLD_NEXT, "free");
  }

  original_free(ptr);
}
