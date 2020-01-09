#include <stdio.h>
#include <stdlib.h>
int main(void){
  int *p;
  int *q;
  int *l;
  p = malloc(10*sizeof(int));
  q = calloc(8,8);
  free(l);
  int check;
  free(q);
  printf("before rand malloc/free 3800\n");
  for(int i = 0; i < 1008000; i++){
    q = malloc((rand()%10000));
    free(q);
    check = i;
  }
  printf("i is now %d\n", check);
printf("before random reallocs 6000");
  for(int i = 0; i < 1000000; i++){
    q = realloc(q, rand() % 10000);
    check = i;
  }
    printf("i is now %d\n", check);
  p = realloc(p,sizeof(int));
  printf("done\n");
  free(p);
  printf("before free null after free something\n");
  free(l);
  printf("after free null");
  return 0;
}
