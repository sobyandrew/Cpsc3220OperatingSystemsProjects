//this is a test file to try out malloc
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
  void *p1, *p2, *p3, *p4, *p5;
  p1 = malloc(1345);
  p2 = malloc(2);
  p3 = malloc(400);
  p4 = malloc(100);
  p5 = malloc(125);//leak
  free(p1);
  //free(p3);
  //free(p4);
  free(p5);

//  printf("Hello world");
}
