#include <stdio.h>
#include <omp.h>

main()
{
  int len=20, b[len], me, i, Nme, g, num_threads, range;
  for(int c=0; c<len;c++) {
  	b[c] = c+1;
  }

  printf("%d\n", (int) (sizeof(b)/sizeof(b[0])) );
  #pragma omp parallel shared(num_threads, range, len, b) private(me, i, Nme, g)
  {
  	me = omp_get_thread_num(); //private
   	num_threads = omp_get_num_threads(); // shared
    range = (len)/num_threads; // shared
	i = me * range, g = 0; // i is starting point where each thread should start storing elements from original array
    Nme = (me+1) * range; // Nme is the ending element where each thread should end storing elements from original array
    printf("%d %d %d\n", me, i, Nme);
   	int *b_me[] = malloc(sizeof(b)/2); // b_me are equivelent to b[] but contain only those element that thread n is responisble for
   	printf("Thread %d has b_me of size %d\n", me, (int)sizeof(b_me));
    // each thread stored its b_me
    for(i; i<Nme; i++) {
      b_me[g] = b[i]+1;
      g++;
    }
	for(g=0;g<range; g++) {
		printf("thread %d array b_me[%d]=%d\n", me, g, b_me[g]);
	}
	int size = (int) (sizeof(b_me)/sizeof(b_me[0]));
	printf("Thread %d has array of size (%d)\n", me, size);
  }
}