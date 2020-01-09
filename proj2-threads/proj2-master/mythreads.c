#include <stdio.h>
#include "mythreads.h"
#include "linkedlist.h"
#include <stdlib.h>
/*#define STACK_SIZE (16*1024)
#define NUM_LOCKS 10
#define CONDITIONS_PER_LOCK 10
*/
//the type of function used to run your threads
//typedef void *(*thFuncPtr) (void *);
#define NUMBER_OF_THREADS 1000

node_t * head = NULL;
//want to think about when i swap to a new thread needs to tell the status and make effective decisions of its status
//keep track of current thread, it has no idea whats happened while its asleep
//linked list head of list is currently running, and pop it to the back of the list when it yields
int currentThread = 0;
void callerFunction(thFuncPtr ptr){

//  interruptsAreDisabled = 0;
  //call function;

}
extern void threadInit(){//initialize data structures in here
  //interruptsAreDisabled = 1;
  //what data structures do i need ? ready running waiting done threads?

  for(int i = 0; i < NUMBER_OF_THREADS; i++){

  }
  //interruptsAreDisabled = 0;
}
extern int threadCreate(thFuncPtr funcPtr, void *argPtr){
//interruptsAreDisabled = 1;
//new thread needs a stack of STACK_SIZE and should execute the funct pointer
//run the thread immediately after
//check in make context
  return 0;
}
//gcc -c to file without a main
extern void threadYield(){
//most of the swap stuff will be done here.
}
extern void threadJoin(int thread_id, void **result){

}

//exits the current thread -- closing the main thread, will terminate the program
extern void threadExit(void *result){

}

extern void threadLock(int lockNum){

}

extern void threadUnlock(int lockNum){

}

extern void threadWait(int lockNum, int conditionNum){

}

extern void threadSignal(int lockNum, int conditionNum){

}

//this
//extern int interruptsAreDisabled;

int main(){
  printf("with thread init currently");
  threadInit();
  head = insert(head,1);
  while(head != NULL){
    printf("%d\n", head->idNum);
    head = head->next;
  }
  printf("Hello World!");
  return 0;
}
