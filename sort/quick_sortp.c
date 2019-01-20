#include <time.h> // for CPU time
#include <sys/time.h> //for gettimeofday
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define max_len 400000
#define LENGTH 80

void partitionArray(double *b, double *c, int beg, int end, int *pivotLoc);
void quickSort(double *b, double *c, int beg, int end);
double find_minimum(double *array, int length);
double find_maximum(double *array, int length);

int main(int argc, char **argv) {
  int i=0,len;
  double b[max_len+1];
  // double *b = malloc(sizeof(double)*max_len+1),
  double c[max_len+1];
  char name[LENGTH]="sort.txt",line[LENGTH];
  FILE *fp;

  // Parallalization starts
  int me, nproc, ierr;
  MPI_Init(&ierr, &argv); // initialise MPI
  MPI_Comm_size(MPI_COMM_WORLD, &nproc); // return total number of processors
  MPI_Comm_rank(MPI_COMM_WORLD, &me); // return number of this processor, me=0..nproc-1
  if(me==0) {
    fp=fopen(name,"r");
    while(1){ //1 serves as true, i.e. condition which is always true
      if(fgets(line, LENGTH,fp)==NULL)break; // finish reading when empty line is read
      if(sscanf(line, "%lf %lf",&b[i],&c[i])==-1) break; // finish reading after error
      i++;
    }

    len=i;fclose(fp);
    printf("Number of items to sort: %i\n",len);
  }
  MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&b, len, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&c, len, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  int size = 0;
  double max = find_maximum(b, len);
  double min = find_minimum(b, len);
  double range = (max-min)/nproc;
  double start = me * range + min;
  double end = (me+1) * range + min;
  double *b_me = malloc(sizeof(double) *len);
  double *c_me = malloc(sizeof(double) *len);

  for(i=0;i<len;i++) {
    if((start <= b[i] && b[i] < end) || (b[i] == end && end == max)) {
      b_me[size] = b[i];
      c_me[size] = c[i];
      size++;
    }
  }
  quickSort(b_me, c_me, 0, size-1);
  if(me==0) {
    MPI_Gatherv(&b_me, size, MPI_DOUBLE, &);
    for(i=0;i<nproc;i++) {
      printf("Thread %d counts %d\n", me, recvcounts[i]);
    }
  }
  // MPI_Gatherv(&b_me, size, MPI_DOUBLE, *sorted_b, size, const int *displs,
  //               MPI_Datatype recvtype, 0, MPI_COMM_WORLD);
  // quickSort(b, c, 0, len-1);  //beg = 0 start index of array end = 5 last index of array

  // writing sorted element of both arrays to sorted.txt
  // fp = fopen("sorted.txt", "w");
  // for(i = 0; i < len; i++){
  //   fprintf(fp, "%lf %lf\n", b[i], c[i]);
  // }
  // fclose(fp);
  MPI_Finalize(); // finalize MPI peacefully (the system would kill the processes otherwise)
  return 0;
}//main() ends here

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

// This is a wrapper function for the recursion process
void quickSort(double *b, double *c, int beg, int end){
  int pivotLoc;
  if(beg < end){
    partitionArray(b, c, beg, end, &pivotLoc);  //this will find the pivot location and partition the array
    quickSort(b, c, beg, pivotLoc - 1);  //quick sort the left sub array
    quickSort(b, c, pivotLoc + 1, end);  //quick sort the right sub array
  }
}//quickSort() ends here

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