#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h> // for CPU time
#include <sys/time.h> //for gettimeofday

#define max_len 400000
#define LENGTH 80


void partitionArray(double *b, double *c, int beg, int end, int *pivotLoc);
void quickSort(double *b, double *c, int beg, int end);
double find_maximum(double *array, int length);
double find_minimum(double *array, int length);

int main(){
  int i=0, j=0, len;
  double b[max_len+1];
  // double *b = malloc(sizeof(double)*max_len+1),
  double c[max_len+1];
  char name[LENGTH]="sort.txt",line[LENGTH];
  FILE *fp;

  fp=fopen(name,"r");
  while(1){ //1 serves as true, i.e. condition which is always true
    if(fgets(line, LENGTH,fp)==NULL)break; // finish reading when empty line is read
    if(sscanf(line, "%lf %lf",&b[i],&c[i])==-1) break; // finish reading after error
    i++;
  }

  len=i;fclose(fp);
  printf("Number of items to sort: %i\n",len);
  int me, nproc, size, offset;
  int *counts = malloc(sizeof(int)*100);
  double *b_me, *c_me, start, end, range, min, max;
  #pragma omp parallel private(i, j, b_me, c_me, start, end, size, offset) shared(nproc, len, b, c, min, max, range, counts)
  {
  	nproc = omp_get_num_threads();
  	min = find_minimum(b, len);
  	max = find_maximum(b, len);
  	range = (max-min)/nproc;
  }
  double sorted_b[nproc * len];
  double sorted_c[nproc * len];
  #pragma omp parallel private(i, j, b_me, c_me, start, end, size, offset) shared(nproc, len, b, c, min, max, range, counts, sorted_b, sorted_c)
  {
  	me = omp_get_thread_num();
  	start = min + me * range;
  	end  = min + (me+1)*range;
  	b_me = malloc(sizeof(double) * len);
  	c_me = malloc(sizeof(double) * len);
  	size=0;
  	for(i=0;i<len;i++) {
  		if ((start <= b[i] && b[i] < end) || (b[i] == min && end == min)) {
  			b_me[size] = b[i];
  			c_me[size] = c[i];
  			size++;
  		}
  	}
  	counts[me] = size;
  	quickSort(b_me, c_me, 0, size-1);
  	// sorted_b = allocate_matrix(nproc, len); 
  	// double *sorted_b = (double *)malloc(nproc * len * sizeof(double));
  	for(j=0;j<size;j++) {
  		offset = me * len + j;
  		sorted_b[offset] = b_me[j];
  		sorted_c[offset] = c_me[j];
  	}
	// now mat[offset] corresponds to m(i, j)

 //  	sorted_b = malloc(nproc * sizeof(double*));
	// sorted_b[me] = b_me;
	// #pragma omp barrier
	// if(me==0) {
		// for(i=0;i<nproc;i++) {
		// 	for(j=0;j<counts[i];j++) {
		// 		offset = i * len +j;
		// 			printf("%lf ", sorted_b[offset]);
		// 	}
		// 		printf("\n");
		// }
	// }
}
fp = fopen("sorted.txt", "w");
for(i=0;i<nproc;i++) {
	for(j=0;j<counts[i];j++) {
		offset = i * len +j;
		fprintf(fp, "%lf %lf\n", sorted_b[offset], sorted_c[offset]);
	}
}
fclose(fp);
  return 0;
}//main() ends here

// This is a wrapper function for the recursion process
void quickSort(double *b, double *c, int beg, int end){
  int pivotLoc;
  if(beg < end){
    partitionArray(b, c, beg, end, &pivotLoc);  //this will find the pivot location and partition the array
    quickSort(b, c, beg, pivotLoc - 1);  //quick sort the left sub array
    quickSort(b, c, pivotLoc + 1, end);  //quick sort the right sub array
  }
}//quickSort() ends here

void partitionArray(double *b, double *c, int beg, int end, int *pivotLoc){
  int left = beg;    //initially left point to the first element of the array
  int right = end;  //initially right point to the last element of the array
  *pivotLoc = left;  //initially pivot point to first element of the array
  double tmp, tmp_c;  //used for swapping values

  while(1){
    //pivot pointing at left
    while(b[*pivotLoc] <= b[right] && *pivotLoc != right){  //pivot element <= right element
      if(b[*pivotLoc] == b[right] && c[*pivotLoc] > c[right]) {
        tmp_c = c[right];
        c[right] = c[*pivotLoc];
        c[*pivotLoc] =tmp_c;
      }
      right--;  //move right one position towards left
    }

    if(*pivotLoc == right){  //both left and right pointing at same element of the array
      break;
    }else if(b[*pivotLoc] > b[right]){
      //pivot element greater than right element. swap pivot and right element.
      tmp = b[right];
      tmp_c = c[right];
      b[right] = b[*pivotLoc];
      c[right] = c[*pivotLoc];
      b[*pivotLoc] = tmp;
      c[*pivotLoc] =tmp_c;
      *pivotLoc = right;  //pivot is now pointing to right
    }

    //pivot pointing to right
    while(b[*pivotLoc] >= b[left] && *pivotLoc != left){  //pivot element >= left element
      if(b[*pivotLoc] == b[left] && c[*pivotLoc] < c[left]) {
        tmp_c = c[left];
        c[left] = c[*pivotLoc];
        c[*pivotLoc] = tmp_c;
      }
      left++;    //move left one position towards right
    }

    if(*pivotLoc == left){  //both left and right pointing at the same element of the array
      break;
    }else if(b[*pivotLoc] < b[left]){
      //pivot element smaller than left element. swap pivot and left element.
      tmp = b[left];
      tmp_c = c[left];
      b[left] = b[*pivotLoc];
      c[left] = c[*pivotLoc];
      b[*pivotLoc] = tmp;
      c[*pivotLoc] = tmp_c;
      *pivotLoc = left;  //pivot is now pointing to left
    }
  }
}//partitionArray() ends here

double find_minimum(double *array, int length) {
  double min=0;
  // int length = (int) (sizeof(array)/sizeof(array[0]));
  for(int i=0;i<length;i++) {
    if(array[i] < min) {
      min = array[i];
    }
  }
  return min;
}

double find_maximum(double *array, int length) {
  double max=0;
  // int length = (int) (sizeof(array)/sizeof(array[0]));
  for(int i=0;i<length;i++) {
    if(array[i] > max) {
      max = array[i];
    }
  }
  return max;
}
