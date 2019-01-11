#include <stdio.h>
#include <omp.h>

int main() {
	int foo=0, me;
	#pragma omp parallel shared(foo)
	{
		me = omp_get_thread_num();
		if (me==0) {
			foo=4;
		}
		printf("thread %d foo is %d\n", me, foo);
	}
	return 0;
}