#include <stdio.h>

double find_size_of_dynamic(double *array, int maximum_bound) {
	int size;
	for(int i=0; i<maximum_bound; i++)
    {
        if(isnan(array[i])) {
			size = i;
            break;
        }
    }
	return size;
}

main() {
	double *dy_array;
  	dy_array = malloc(sizeof(double));
  	printf("%f\n", dy_array[10]);

  	// fill that array
  	for (int i=0;i<20;i++) {
  		dy_array[i] = i+1;
  		printf("%f\n", dy_array[i]);
  	}
    
    printf("size of dynamic array %f\n", find_size_of_dynamic(dy_array, 40));
}