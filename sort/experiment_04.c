#include <stdio.h>
#include <omp.h>

main()
{
    int len = 16;
  #pragma omp ordered 
    {
      int me = omp_get_thread_num();
      printf("thread %d hi\n", me);
    }
}