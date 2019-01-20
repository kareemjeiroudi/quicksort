#include <stdio.h>

int main() {
	// int numbers [4][4];
	// int array01[4] = {1, 2, 3, 4};
	// int array02[4] = {5, 6, 7, 8};
	// int array03[4] = {9, 10, 11, 12};
	// int array04[4] = {13, 14 ,15, 16};
	int numbers[2][4] = {{1, 2, 3, 4},
			5, 6, 7, 8};

	// numbers[0] = array01;
	// numbers[1] = array02;
	// numbers[2] = array03;
	// numbers[3] = array04;
	int g=0;
	for(int i=0;i<2;i++) {
		for(int j=0; j<4;j++) {
			printf("%d  ", numbers[i][j]);
		}
		printf("\n");
		// numbers[i] = i+1;
		// g++;
		// printf("%d\n", numbers[i]);
		// if(g==4) {
		// 	g=0;
		// 	printf("\n");
		// }

	}
}