//sort.c
#include <time.h> // for CPU time
#include <sys/time.h> //for gettimeofday
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define max_len 400000
#define LENGTH 80

/* Takes a dynamic array and maximum expected boundary,
 * returns the real size of tha array
 */
// int find_size_of_dynamic(double *array, int maximum_bound) {
//   int size;
//   for(int i=0; i<maximum_bound; i++) {
//     // once the array doesn't return proper value, break!
//     if(isnan(array[i])) {
//        size = i;
//        break;
//     }
//   }
//   return size;
// }

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

main() {
int i=1,len,ind[max_len+1],j,cur,prev;
double b[max_len+1],c[max_len+1],new,cnew,time;
char name[LENGTH]="sort.txt",line[LENGTH];
FILE *fp;
clock_t cpu0,cpu1,cpu2,cpu3; // clock_t defined in <time.h> and <sys/types.h> as int
struct timeval time0, time1,time2,time3; // for wall clock in s and us
double  dtime12,dtime03; // for wall clock in s (real number)

cpu0 = clock();    // assign initial CPU time (IN CPU CLOCKS)
gettimeofday(&time0, NULL); // returns structure with time in s and us (microseconds)
//printf("Input filename\n"); scanf("%s",name);
fp=fopen(name,"r");
while(1){ //1 serves as true, i.e. condition which is always true
  if(fgets(line, LENGTH,fp)==NULL)break; // finish reading when empty line is read
  if(sscanf(line, "%lf %lf",&b[i],&c[i])==-1) break; // finish reading after error
  i++;
}
len=i-1;fclose(fp);
printf("Number of items to sort: %i\n",len);


/* split b into postive and negative ones, where all are shared variables
 * this way, it's also a shortcut for sorting, because we skip the part of comparing
 * positive values with negative ones.
 * 
 * Each will be sorted independetly 
 */
double *b_positive = malloc(sizeof(double)*len);
double *b_me_positive;
int *b_positive_counts = malloc(sizeof(int)*len); // shared on the contrary
double *b_negative = malloc(sizeof(double)*len);
double *b_me_negative;
int *b_negative_counts;

double *c_positive = malloc(sizeof(double)*len);
double *c_me_positive;
int *c_positive_counts = malloc(sizeof(int)*len); // shared on the contrary
double *c_negative = malloc(sizeof(double)*len);
double *c_me_negative;
int *c_negative_counts = malloc(sizeof(int)*len);

int *ind_me_positives;
int *ind_me_negatives;
/* b_pos = num of positve elements in the whole array
 * b_neg = likewise but of negative elements
 * ele = is nothing but an iterator private to each thread
 */
int b_pos=0, b_neg=0, ele=0;
int c_pos=0, c_neg=0;

/* split the positve numbers from the negative ones globally*/
for(ele=0; ele<len+1;ele++) {
  if(b[ele]>0) {
    b_positive[b_pos] = b[ele];
    b_pos++;
  }
  else if(b[ele] <0) {
    b_negative[b_neg] = b[ele];
    b_neg++;
  }
} // now b_postive has all positive elements from b and b_negative all negatives from b
  // the same will happen to c

// now it c turn
for(ele=0; ele<len+1;ele++) {
  if(c[ele]>0) {
    c_positive[c_pos] = c[ele];
    c_pos++;
  }
  else if(c[ele]<0) {
    c_negative[c_neg] = c[ele];
    c_neg++;
  }
}
printf("c_positive has %d\n", c_pos);

/* the parallel region */
int me, num_threads;
/* start and end are private to each thread
 * the four of these variables will help with defining the intervals of each thread
 */
double b_start, c_start, b_end, c_end, b_max, b_min, c_max, c_min;
b_max = find_maximum(b_positive, b_pos);
b_min = find_minimum(b_negative, b_neg);
c_max = find_maximum(c_positive, c_pos);
c_min = find_minimum(c_negative, c_neg);
printf("This is b_min %lf\n", b_min);
printf("This is b_max %lf\n", b_max);
printf("This is c_min %lf\n", c_min);
printf("This is c_max %lf\n", c_max);
printf("b_pos is %d\n", b_pos);
printf("b_neg is %d\n", b_neg);
printf("c_pos is %d\n", c_pos);
printf("c_neg is %d\n", c_neg);
#pragma omp parallel private(me, b_start, b_end, c_start, c_end, ele, \
b_me_positive, c_me_positive, b_me_negative, c_me_negative, \
 cpu1, cpu2, cpu3, time1, time2, time3) \
shared(num_threads, b_max, b_min, c_max, c_min, b_pos, b_neg, \
 b_positive_counts, b_negative_counts, c_positive_counts, c_negative_counts)
{
  num_threads = omp_get_num_threads();
  me = omp_get_thread_num();
  // the starting point of each thread
  b_start = me * (b_max/num_threads +1) - me;
  b_end = (me+1) * (b_max/num_threads +1) - (me+1);
  printf("Postives: thread %d has b_start %lf and b_end %lf \n", me, b_start, b_end);
  /* each thread will take its share of the postives */
  b_me_positive = malloc(sizeof(double)*len);
  b_positive_counts[me] = 0;
  for(ele=0;ele<b_pos;ele++) {
    if(b_positive[ele] >= b_start & b_positive[ele] <= b_end) {
      b_me_positive[b_positive_counts[me]] = b_positive[ele];
      b_positive_counts[me]++;
    }
  }
  #pragma omp barrier  
  c_start = me * (c_max/num_threads +1) - me;
  c_end= (me+1) * (c_max/num_threads +1) - (me+1);
  printf("Postives: thread %d has c_start %lf and c_end %lf \n", me, c_start, c_end);
  c_me_positive = malloc(sizeof(double)*len);
  c_positive_counts[me] = 0;
  for(ele=0;ele<c_pos;ele++) {
    if(c_positive[ele] >= c_start & c_positive[ele] <= c_end) {
      c_me_positive[c_positive_counts[me]] = c_positive[ele];
      c_positive_counts[me]++;
    }
  }
  #pragma omp barrier   
  printf("thread %d has %d b_positives to sort\n", me, b_positive_counts[me]);
  #pragma omp barrier  
  printf("thread %d has %d c_positives to sort\n", me, c_positive_counts[me]);

  if(me == 0) {
    int b_pos_count = 0, c_pos_count = 0;
    for(ele=0; ele<num_threads; ele++) {
      b_pos_count += b_positive_counts[ele];
      c_pos_count += c_positive_counts[ele];
    }
    printf("b_positives are %d\n", b_pos_count);
    printf("c_positives are %d\n", c_pos_count);
  }

// these two should be for 1k
// b_pos 513
// b_neg 487
    cpu1 = clock();    // assign initial CPU time (IN CPU CLOCKS)
    gettimeofday(&time1, NULL); // returns structure with time in s and us (microseconds)
    ind_me_positives = malloc(sizeof(int)*len);
    ind_me_positives[0]=1;
    for(j=2;j<=b_positive_counts[me];j++){ // start sorting with the second item
      new=b_me_positive[j];cnew=c_me_positive[j];cur=0;
      for(i=1;i<j;i++){
        prev=cur;cur=ind_me_positives[cur];
        if(new==b_me_positive[cur]){printf("Equal numbers %lf\n",new);}
        if((new<b_me_positive[cur]) | ((new==b_me_positive[cur])&(cnew<c_me_positive[cur]))){ind_me_positives[prev]=j;ind_me_positives[j]=cur;goto loop;}
      }
      // new number is the largest so far
      ind_me_positives[cur]=j;
      loop: ;
    }

  
  // cpu1 = clock();    // assign initial CPU time (IN CPU CLOCKS)
  // gettimeofday(&time1, NULL); // returns structure with time in s and us (microseconds)
  // ind[0]=1;
  // for(j=2;j<=len;j++){ // start sorting with the second item
  //   new=b[j];cnew=c[j];cur=0;
  //   for(i=1;i<j;i++){
  //     prev=cur;cur=ind[cur];
  //     if(new==b[cur]){printf("Equal numbers %lf\n",new);}
  //     if((new<b[cur]) | ((new==b[cur])&(cnew<c[cur]))){ind[prev]=j;ind[j]=cur;goto loop;}
  //   }
  //   // new number is the largest so far
  //   ind[cur]=j;
  //   loop: ;
  // }
    cpu2 = clock();    // assign CPU time (IN CPU CLOCKS)
    gettimeofday(&time2, NULL);
    dtime12 = ((time2.tv_sec  - time1.tv_sec)+(time2.tv_usec - time1.tv_usec)/1e6);
    printf("Elapsed wall time sorting         CPU time\n");
    printf("Duration 12 %f %f\n", dtime12,(float) (cpu2-cpu1)/CLOCKS_PER_SEC);
    #pragma omp barrier
    // printing
    cur=0;
    if(me==0) {
      fp=fopen("sorted.txt","w");
    } else
    {
      fp=fopen("sorted.txt", "a");
    }
    for(i=1;i<=b_positive_counts[me];i++){cur=ind_me_positives[cur];fprintf(fp,"%lf %lf\n",b_me_positive[cur],c_me_positive[cur]);}
    if(me==0){
      fclose(fp);
    }
    cpu3 = clock();    // assign initial CPU time (IN CPU CLOCKS)
    gettimeofday(&time3, NULL); // returns structure with time in s and us (microseconds)
    dtime03 = ((time3.tv_sec  - time0.tv_sec)+(time3.tv_usec - time0.tv_usec)/1e6);
    printf("Elapsed wall time complete         CPU time\n");
    printf("Duration 03 %f %f\n", dtime03,(float) (cpu3-cpu0)/CLOCKS_PER_SEC);
  } /* End of Parallel Region */
}

