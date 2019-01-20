#include <stdio.h>
#include <omp.h>

main() 
{
 #pragma omp parallel
  {
    #pragma omp ordered
    printf("%d says hello world\n", omp_get_thread_num());
  }
}
