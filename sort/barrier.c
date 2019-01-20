#include <stdio.h>

#include <omp.h>

main() {
	#pragma omp parallel
	{
		printf("Hello World!\n");

		// #pragma omp barrier
		printf("Other side\n" );
	}
}