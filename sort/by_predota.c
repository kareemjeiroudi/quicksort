// simple program to dynamically allocate array of length count, i.e. [0..count-1]
#include <stdio.h>
#include <stdlib.h>
int main()
{
  int i,count; double *number;

  printf("Enter number of items: "); scanf("%i",&count);
  number = malloc(sizeof(double));
  for(i=1; i<=count; i++) {
    number[i-1]=-i;
    printf("%4i. number: %lf\n", i,number[i-1]);
  }
  return 0;
}