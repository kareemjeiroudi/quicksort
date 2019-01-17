#include <time.h> // for CPU time
#include <sys/time.h> //for gettimeofday
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define max_len 400000
#define LENGTH 80

// void partitionArray(int *b, int beg, int end, int *pivotLoc);
void partitionArray(double *b, int beg, int end, int *pivotLoc){
  int left = beg;    //initially left point to the first element of the array
  int right = end;  //initially right point to the last element of the array
  *pivotLoc = left;  //initially pivot point to first element of the array
  double tmp;  //used for swapping values

  while(1){

    //pivot pointing at left
    while(b[*pivotLoc] <= b[right] && *pivotLoc != right){  //pivot element <= right element
      right--;  //move right one position towards left
    }

    if(*pivotLoc == right){  //both left and right pointing at same element of the array
      break;
    }else if(b[*pivotLoc] > b[right]){
      //pivot element greater than right element. swap pivot and right element.
      tmp = b[right];
      b[right] = b[*pivotLoc];
      b[*pivotLoc] = tmp;
      *pivotLoc = right;  //pivot is now pointing to right
    }

    //pivot pointing to right
    while(b[*pivotLoc] >= b[left] && *pivotLoc != left){  //pivot element >= left element
      left++;    //move left one position towards right
    }

    if(*pivotLoc == left){  //both left and right pointing at the same element of the array
      break;
    }else if(b[*pivotLoc] < b[left]){
      //pivot element smaller than left element. swap pivot and left element.
      tmp = b[left];
      b[left] = b[*pivotLoc];
      b[*pivotLoc] = tmp;
      *pivotLoc = left;  //pivot is now pointing to left
    }
  }
}//partitionArray() ends here

// void quickSort(int *b, int beg, int end);
void quickSort(double *b, int beg, int end){
  int pivotLoc;
  if(beg < end){
    partitionArray(b, beg, end, &pivotLoc);  //this will find the pivot location and partition the array
    quickSort(b, beg, pivotLoc - 1);  //quick sort the left sub array
    quickSort(b, pivotLoc + 1, end);  //quick sort the right sub array
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

int main(){
  int i=0,len,ind[max_len+1],j,cur,prev;
  double b[max_len+1],c[max_len+1],new,cnew,time;
  char name[LENGTH]="sort.txt",line[LENGTH];
  FILE *fp;
  clock_t cpu0,cpu1,cpu2,cpu3; // clock_t defined in <time.h> and <sys/types.h> as int
  struct timeval time0, time1,time2,time3; // for wall clock in s and us
  double  dtime12,dtime03; // for wall clock in s (real number)
  
  fp=fopen(name,"r");
  while(1){ //1 serves as true, i.e. condition which is always true
    if(fgets(line, LENGTH,fp)==NULL)break; // finish reading when empty line is read
    if(sscanf(line, "%lf %lf",&b[i],&c[i])==-1) break; // finish reading after error
    i++;
  }
  len=i;fclose(fp);
  printf("Number of items to sort: %i\n",len);

  // KAREEM
  double *b_me; 
  double *c_me;
  int *counts = malloc(sizeof(int)* len);
  double start, end;
  double max = find_maximum(b, len+1);
  double min = find_minimum(b, len+1);
  printf("max = %lf and min = %lf\n", max, min);
  /* the parallel region */
  int me, num_threads, pos, neg;
  /* start and end are private to each thread
   * the four of these variables will help with defining the intervals of each thread
   */
  #pragma omp parallel private(me, start, end, i, j, new, cnew, cur, prev, pos, neg, b_me, c_me,\
   cpu1, cpu2, cpu3, time1, time2, time3, fp) shared(num_threads, counts, max, min, len, b, c) 
  {
    num_threads = omp_get_num_threads();
    me = omp_get_thread_num();
    b_me = malloc(sizeof(double)* len);
    c_me = malloc(sizeof(double)* len);
    start = me * (min/num_threads +1) - me;
    end = (me+1) * (min/num_threads +1) - (me+1);
    printf("Thread %d negatively has start %lf and end %lf\n", me, start, end);
    
    neg = 0;
    for(i=0;i<len;i++) {
      if((b[i] < 0 && b[i] <= start && b[i] > end) || (b[i] == min && end == min)){
        b_me[neg] = b[i];
        c_me[neg] = c[i];
        neg++;
      }
    }
    #pragma omp barrier
    // now the positves
    start = me * (max/num_threads +1) - me;
    end = (me+1) * (max/num_threads +1) - (me+1);
    pos = 0;
    for(i=0;i<len;i++) {
      if((b[i] >= 0 && b[i] >= start && b[i] < end) || (b[i] == max && end == max)) {
        b_me[neg+pos] = b[i];
        c_me[neg+pos] = c[i];
        pos++;
      }
    }
    counts[me] = neg+pos;
    #pragma omp barrier
    printf("Report of Thread %d\nCounts = %d, pos = %d, neg = %d\nstart = %lf and end = %lf\n",\
     me, counts[me], pos, neg, start, end);
    quickSort(b_me, 0, counts[me]-1);  //beg = 0 start index of array end = 5 last index of array
    if(me==0) {
      fp=fopen("sorted.txt","w");
    } else
    {
      fp=fopen("sorted.txt", "a");
    }

      for(i=0;i< neg;i++){
        fprintf(fp,"%lf\n",b_me[i]);
      }
      #pragma omp barrier
      for(i=neg;i<counts[me];i++){
        fprintf(fp,"%lf\n",b_me[i]);
      }
	#pragma omp barrier
    if(me==0){
      fclose(fp);
    }
  }

  

  // printing sorted element of array
  // for(i = 0; i < SIZE; i++){
  //   printf("%d\t", b[i]);
  // }
  // printf("\n");
  return 0;
}//main() ends here

