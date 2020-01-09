#include <stdio.h>
#include <stdlib.h>
void main(void)
{
  int *p1, *p2;
  p1 = malloc(36);
  p2 = p1;
  if(p1 == p2)
  {
    printf("in if p1 is %p and p2 is %p", p1,p2);
  }else{
    printf("in else p1 is %p and p2 is %p", p1,p2);
  }
}
