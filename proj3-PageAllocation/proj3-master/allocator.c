//Andrew Soby Project 3 asoby C28319723 allocator.c
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PAGESIZE 4096
#define NUMBERTOALLOC 10000

int initialize = 0;
int counter = 0;
int recentFree = -1;
typedef struct allocated{
  void * ptr;
  size_t size;
} allocated_t;

allocated_t trackAllocater [NUMBERTOALLOC];

void * malloc(size_t size){
  //write(1, "ma", 3);
  //void * ptr;
  //int findSize = 0;
  if(initialize == 0){
    initialize = 1;
    for(int i = 0; i < NUMBERTOALLOC; i++){
      trackAllocater[i].size = -1;
      trackAllocater[i].ptr = NULL;
    }
  }

  if(size >1024){
    int i = 1;

    while((int)size > (i * PAGESIZE)){
      i++;
    }

    void *ptr = mmap(0, (i*PAGESIZE), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if((recentFree != -1) && (recentFree >= 0 && recentFree <= NUMBERTOALLOC)){
      trackAllocater[recentFree].size = i*PAGESIZE;
      trackAllocater[recentFree].ptr = ptr;
      recentFree = -1;
      return ptr;
    }
    else{
      trackAllocater[counter].size = i*PAGESIZE;
      trackAllocater[counter].ptr = ptr;
      counter++;
      return ptr;
    }
  }
  else{
    void *ptr = mmap(0, (PAGESIZE), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if((recentFree != -1) && (recentFree >= 0 && recentFree <= NUMBERTOALLOC)){
      trackAllocater[recentFree].size = PAGESIZE;
      trackAllocater[recentFree].ptr = ptr;
      recentFree = -1;
      return ptr;
    }
    else{
      trackAllocater[counter].size = PAGESIZE;
      trackAllocater[counter].ptr = ptr;
      counter++;
      return ptr;
    }
  }

  //return ptr;
}

void free(void * ptr){
  //write(1, "fr", 3);
  if(ptr == NULL){
    return;
  }
  size_t foundSize = 0;
  for(int i = 0; i < counter; i++){
    if(trackAllocater[i].ptr == ptr){
      foundSize = trackAllocater[i].size;
      trackAllocater[i].size = -1;
      trackAllocater[i].ptr = NULL;
      recentFree = i;
      break;
    }
  }
  munmap(ptr, foundSize);//FIXME: need to have the pointer size
}

void * calloc(size_t init, size_t size){
  void * ptr;
  ptr = malloc(init * size);
  memset(ptr, 0, init*size);
  return ptr;
}

void * realloc(void * ptr, size_t size){
  //write(1, "re", 3);
  //if(size < (size_t)0) return NULL;
  void * newPtr = malloc(size);
  if(newPtr == NULL) return NULL;


  size_t foundSize = 0;
  for(int i = 0; i < counter; i++){
    if(trackAllocater[i].ptr == ptr){
      foundSize = trackAllocater[i].size;
      break;
    }
  }
  //if size of newptr is smaller than old ptr
  if(size < foundSize)//FIXME: how will i get the size of old pointer
  {
      memcpy(newPtr, ptr, size);
      free(ptr);
  }
  else{
      memcpy(newPtr, ptr, foundSize);//FIXME: same fix as above need the size of old ptr block
      free(ptr);
  }

  return newPtr;
}
