//Andrew Soby asoby C28319723 Cpsc 3220 Project 1 sctracer.c
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <stdbool.h>

#define NUMBEROFSYSCALLS 512 //Number of syscalls wont be higher than this

int main(int argc, char **argv)
{
  //split up argv[1] on first space
  char *str = strtok(argv[1], " ");
  //counter to track indexes for argsToPass
  int counter = 0;
  //create array of strings to store arguments in argv[1]
  char ** argsToPass = NULL;
  bool infLoop = true;

  while(str != NULL){
    //need to reallocate so that we dont run out of space
    //use counter +1 because counter is used for indexing of arrays
    //which starts at 0 index so anything times 0 = 0 which we dont want
    argsToPass = realloc(argsToPass, sizeof(char*) *(counter + 1));
    argsToPass[counter] = str;
    counter++;
    //keep reading until str tok is NULL
    str = strtok(NULL, " ");
  }

  //need to reallocate for an end character
  //use counter + 1 again because it isues for indexing of arrays
  //so the +1 accounts for that and allocates another string to be NULL
  //at the end
  argsToPass = realloc(argsToPass, sizeof(char*) *(counter + 1));
  argsToPass[counter] = 0; // set the last one to empty so we dont go out of bounds

  //create an array of our system calls to track.
  //keep track of the times a system call is called
  int matchingSysNums[NUMBEROFSYSCALLS];

  //initiate everything to 0
  for(int i = 0; i < NUMBEROFSYSCALLS; i++){
    matchingSysNums[i] = 0;
  }

  //open up a file we will write to
  FILE *file_ptr = fopen(argv[2], "w");
  //fork and get child pid
  pid_t child = fork();

  if(child == 0){
    //in child now so trace it
    ptrace(PTRACE_TRACEME);
    //get the child pid for the parent 
    child = getpid();
    execvp(argsToPass[0], argsToPass);
    // exec calls executable, then executable and its args without outputfile.
  } else{
    int status;
    int syscall_num = 0;

    //set options to trace sys syscalls
    ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);
    //infLoop = false;
    while(infLoop){
      //wait for the child to change status
      waitpid(child, &status, 0);

      //if stopped or status changed
      if(!(WIFSTOPPED(status) && WSTOPSIG(status)& 0x80)){
        syscall_num = ptrace(PTRACE_PEEKUSER, child, sizeof(long) *ORIG_RAX, NULL);
        matchingSysNums[syscall_num]++;
      }

      //break from the loop since the child process called exit
      if(WIFEXITED(status)){
        break;
      }

      //continue to trace the system calls of child
      ptrace(PTRACE_SYSCALL, child, 0,0);
    }
  }

  for(int i = 0; i < NUMBEROFSYSCALLS; i++){
    if(matchingSysNums[i] > 0){
      //this is to account for double accounting when it checks the syscall twice
      if(matchingSysNums[i] % 2 == 0){
        fprintf(file_ptr,"%d\t%d\n", i, matchingSysNums[i]/2);
      }else{
        //if it reaches this else its the case where a double count didnt occur
        fprintf(file_ptr,"%d\t%d\n", i, matchingSysNums[i]);
      }
    }
  }

  fclose(file_ptr);
  return 0;
}
