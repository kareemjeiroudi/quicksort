#include <stdio.h>
#include <omp.h>

main() 
{
 FILE *fp;
 int b[10];
 #pragma omp parallel
 {
  int me = omp_get_thread_num();
  if(me==0) {
   fp=fopen("printed.txt","w");
  } else {
   fp=fopen("printed.txt","a");
  }
  int number = 1 + me;
  #pragma omp for ordered private(me) // private(i) is implicit
  for(int i=0;i<10;i++) {
   printf("thread %d prints %d\n", me, i);
   fprintf(fp, "thread %d prints %d\n", me, i);
   b[i] = me * me * i;
  }
  if(me==0) {
   fclose(fp);
  }

  printf("The array\n");
   for(int i=0;i<10;i++){
    printf("%d  ", b[i]);
   }
 }
 printf("\n");
 int i = 3,Nme= 10;
 for(i;i<Nme;i++){
  printf("%d\n", i);
 }
}
