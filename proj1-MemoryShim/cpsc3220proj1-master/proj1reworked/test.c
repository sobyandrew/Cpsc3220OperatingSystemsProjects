//this is a test file to try out malloc
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
  void *p1, *p2, *p3;
  p1 = malloc(1345);
  p2 = malloc(2);
  p3 = malloc(400);
  free(p2);
  free(p3);
  printf("Hello world");
}
