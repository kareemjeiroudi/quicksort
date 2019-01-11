//sort.c
#include <time.h> // for CPU time
#include <sys/time.h> //for gettimeofday
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define max_len 400000
#define LENGTH 40

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

main(){
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
double *b_positive = malloc(sizeof(double));
double *b_me_positive = malloc(sizeof(double)); // private to each thread
int *b_positive_counts = malloc(sizeof(int)); // shared on the contrary
double *b_negative = malloc(sizeof(double));
int *b_negative_counts = malloc(sizeof(int));
int pos=0, neg=0, ele=0;
for(ele=0; ele<len+1;ele++) {
  if(b[ele]>0) {
    b_positive[pos] = b[ele];
    pos++;
  }
  else if(b[ele] <0) {
    b_negative[neg] = b[ele];
    neg++;
  }
}
int me, num_threads;
double start, end, max, min;
max = find_maximum(b_positive, pos);
min = find_minimum(b_negative, neg);
printf("This is min %lf\n", min);
printf("This is max %lf\n", max);
/* the parallel region */
#pragma omp parallel private(me, start, end, ele, b_me_positive) shared(b_positive_counts, num_threads, max, min, pos, neg)
{
  num_threads = omp_get_num_threads();
  me = omp_get_thread_num();
  printf("This is me %d\n", me);
  // the starting point of each thread
  start = me * (max/num_threads +1);
  end = (me+1) * (max/num_threads +1);
  printf("thread %d has start %lf and end %lf \n", me, start, end);
  b_positive_counts[me] = 0;
  for(ele=0;ele<pos;ele++) {
    if(b_positive[ele] >= start & b_positive[ele] < end) {
      b_me_positive[b_positive_counts[me]] = b_positive[ele];
      b_positive_counts[me]++;
    }
  }

  printf("\n");
  printf("thread %d has %d b_positives to sort\n", me, b_positive_counts[me] );
}
// pos 513
// neg 487

//   // each thread finds the size of its array
//   size = find_size_of_dynamic(b_me, len+1);
//   double b_me[range];
//   ind[0]=1;
//   for(g=2;g<=size;g++) { // start sorting with the second item
//     new=b_me[g];cnew=c_me[g];cur=0;
//     for(k=1;k<g;k++){
//       prev=cur;cur=ind[cur];
//       if(new==b_me[cur]){printf("Equal numbers %lf\n",new);}
//       if((new<b_me[cur]) | ((new==b_me[cur])&(cnew<c_me[cur]))){ind[prev]=g;ind[g]=cur;goto loop;}
//     }
//     // new number is the largest so far
//     ind[cur]=j;
//     loop: ;
//   }
  
  cpu1 = clock();    // assign initial CPU time (IN CPU CLOCKS)
  gettimeofday(&time1, NULL); // returns structure with time in s and us (microseconds)
  ind[0]=1;
  for(j=2;j<=len;j++){ // start sorting with the second item
    new=b[j];cnew=c[j];cur=0;
    for(i=1;i<j;i++){
      prev=cur;cur=ind[cur];
      if(new==b[cur]){printf("Equal numbers %lf\n",new);}
      if((new<b[cur]) | ((new==b[cur])&(cnew<c[cur]))){ind[prev]=j;ind[j]=cur;goto loop;}
    }
    // new number is the largest so far
    ind[cur]=j;
    loop: ;
  }
  cpu2 = clock();    // assign CPU time (IN CPU CLOCKS)
  gettimeofday(&time2, NULL);
  dtime12 = ((time2.tv_sec  - time1.tv_sec)+(time2.tv_usec - time1.tv_usec)/1e6);
  printf("Elapsed wall time sorting         CPU time\n");
  printf("Duration 12 %f %f\n", dtime12,(float) (cpu2-cpu1)/CLOCKS_PER_SEC);
// } /* End of Parallel Region */
cur=0;
fp=fopen("sorted.txt","w");
for(i=1;i<=len;i++){cur=ind[cur];fprintf(fp,"%lf %lf\n",b[cur],c[cur]);}
fclose(fp);
cpu3 = clock();    // assign initial CPU time (IN CPU CLOCKS)
gettimeofday(&time3, NULL); // returns structure with time in s and us (microseconds)
dtime03 = ((time3.tv_sec  - time0.tv_sec)+(time3.tv_usec - time0.tv_usec)/1e6);
printf("Elapsed wall time complete         CPU time\n");
printf("Duration 03 %f %f\n", dtime03,(float) (cpu3-cpu0)/CLOCKS_PER_SEC);
}

