#include <stdio.h>
#include <omp.h>

main() {
	int counts[4];
	int me;
	#pragma omp parallel shared(counts) private(me)
	{
		me = omp_get_thread_num();
		int i = 2;
		counts[me] = i*me;
	}

	for(int i=0; i<4; i++) {
		printf("count %d = %d\n", i, counts[i]);
	}
}