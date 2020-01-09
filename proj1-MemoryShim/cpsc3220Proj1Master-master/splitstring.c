#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char **argv){
  char *str = strtok(argv[1], " ");
  int counter = 0;
  char ** argsToPass = NULL;

  while(str){
    counter++;
    argsToPass = realloc(argsToPass, sizeof(char*) *counter);
    argsToPass[counter -1] = str;
    str = strtok(NULL, " ");
  }
  argsToPass = realloc(argsToPass, sizeof(char*) *(counter + 1));
  argsToPass[counter] = 0;
  
  for(int i = 0; i < counter; i++){
    printf("argsToPass %d is %s\n",i ,argsToPass[i]);
  }
  return 0;
}
