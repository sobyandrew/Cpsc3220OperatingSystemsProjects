//Andrew Soby asoby C28319723 Cpsc 3220 Project 1 leakcount.
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv, char **envp){
  if(fork() == 0){//fork to child and parent
    //increment the arguments array pointer to start at executable for exec 
    argv = argv + 1;

    //im in the child run execvp
    //setenv("LD_PRELOAD=./memory_shim.so");
    //set the environmental variable
    putenv("LD_PRELOAD=./memory_shim.so");
    int i = execvp(argv[0], argv);
    if(i < 0)
      perror("execvp"); //error in execvpe if this occurs
  }else{
    //parent waits for child to finish
    wait(NULL);
  }
  return 0;
}
