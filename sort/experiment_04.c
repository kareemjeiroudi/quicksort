#include <stdio.h>
#include <omp.h>

void add_three(int *number) {
	*number =  1;
}

main()
{
 #pragma omp parallel
	{
		int me = omp_get_thread_num();
		int number = me+1;
		add_three(&number);
		printf("Thread %d %d\n", me, number);
	}
}