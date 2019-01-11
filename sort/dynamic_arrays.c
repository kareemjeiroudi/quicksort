#include <stdio.h>

main() {
	double *dy_array;
  	dy_array = malloc(sizeof(double));
  	printf("%f\n", dy_array[10]);

  	// fill that array
  	for (int i=0;i<20;i++) {
  		dy_array[i] = i+1;
  		printf("%f\n", dy_array[i]);
  	}

  	int len = 0;
    for(int i= 0; i<30; i++)
    {
    	printf("%d\n", i);
        if(isnan(dy_array[i])) {
			len = i;
            break;
        }
        else if(i==21 | i==22 | i==23) {
        	printf("\n");
        	printf("i is %f\n", dy_array[i]);
        }
    }
    printf("%f\n", dy_array[10]);
    printf("size of dynamic array %d\n", len);
}