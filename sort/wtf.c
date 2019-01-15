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

// KAREEM
double *b_me; 
double *c_me;
int *ind_me;
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
// #pragma omp threadprivate(ind_me, b_me, c_me)
#pragma omp parallel private(me, start, end, i, j, new, cnew, cur, prev, pos, neg, ind_me, b_me, c_me,\
 cpu1, cpu2, cpu3, time1, time2, time3) shared(num_threads, counts, max, min, len, b, c) 
{
  num_threads = omp_get_num_threads();
  me = omp_get_thread_num();
  b_me = malloc(sizeof(double)* len);
  c_me = malloc(sizeof(double)* len);
  ind_me = malloc(sizeof(int)* len);
  start = me * (min/num_threads +1) - me;
  end = (me+1) * (min/num_threads +1) - (me+1);
  printf("Thread %d negatively has start %lf and end %lf\n", me, start, end);
  neg = 0;
  // the negatives
  for(i=1;i<=len;i++) {
    if((b[i] < 0 && b[i] <= start && b[i] > end) || (b[i] == min && end == min)){
      b_me[neg] = b[i];
      c_me[neg] = c[i];
      neg++;
    }
  }
  if(end==min) {
    printf("HEY!\n");
  }
  #pragma omp barrier
  // now the positves
  start = me * (max/num_threads +1) - me;
  end = (me+1) * (max/num_threads +1) - (me+1);
  pos = 0;
  if(end == max) {
    printf("HEY!\n");
  }
  for(i=1;i<=len;i++) {
    if((b[i] >= 0 && b[i] >= start && b[i] < end) || (b[i] == max && end == max)) {
      b_me[neg+pos] = b[i];
      c_me[neg+pos] = c[i];
      pos++;
    }
    else if (end == max) {
      printf("FUCK!\n");
    }
  }
  counts[me] = neg+pos;
  #pragma omp barrier
  // each thread report your thread to the terminal
  printf("Report of Thread %d\nCounts = %d, pos = %d, neg = %d\nstart = %lf and end = %lf\n",\
    me, counts[me], pos, neg, start, end);
  // #pragma omp barrier
  // for(i=0;i<counts[me];i++) {
  //   printf("thread %d %d. = %lf\n",me, i+1, b_me[i]);
  // }
  #pragma omp barrier
  if(me==0){
    printf("The counts are:\n");
    for(i=0;i<4;i++) {
      printf("%d %d\n",i, counts[i]);
    }
  }
  #pragma omp barrier
  printf("%lf %lf %d\n", end, max, end==max);
  #pragma omp barrier
    cpu1 = clock();    // assign initial CPU time (IN CPU CLOCKS)
    gettimeofday(&time1, NULL); // returns structure with time in s and us (microseconds)
    int *ind_me = malloc(sizeof(int)*counts[me]);
    ind_me[0]=1;
    for(j=2;j<=counts[me];j++){ // start sorting with the second item
      new=b_me[j];cnew=c_me[j];cur=0;
      for(i=1;i<j;i++){
        prev=cur;cur=ind_me[cur];
        if(new==b_me[cur]){printf("Equal numbers %lf\n",new);}
        if((new<b_me[cur]) | ((new==b_me[cur])&(cnew<c_me[cur]))){ind_me[prev]=j;ind_me[j]=cur;goto loop;}
      }
      // new number is the largest so far
      ind_me[cur]=j;
      loop: ;
    }

    cpu2 = clock();    // assign CPU time (IN CPU CLOCKS)
    gettimeofday(&time2, NULL);
    dtime12 = ((time2.tv_sec  - time1.tv_sec)+(time2.tv_usec - time1.tv_usec)/1e6);
    printf("Elapsed wall time sorting         CPU time\n");
    printf("Duration 12 %f %f\n", dtime12,(float) (cpu2-cpu1)/CLOCKS_PER_SEC);
    #pragma omp barrier
    // }
    /* printing */
    // #pragma omp parallel ordered private(cure, me)
    cur=0;
    if(me==0) {
      fp=fopen("sorted.txt","w");
    } else
    {
      fp=fopen("sorted.txt", "a");
    }
    if (me==0) {
    for(i=1;i<=counts[me];i++) {
      cur=ind_me[cur];
      if(b_me[cur] < 0) {
        fprintf(fp,"%lf %lf\n",b_me[cur],c_me[cur]);
      }
    }
     for(i=1;i<=counts[me];i++){
      cur=ind_me[cur];
      if(b_me[cur] > 0) {
        fprintf(fp,"%lf %lf\n",b_me[cur],c_me[cur]);
      }
    }
    }
    
    #pragma omp barrier
    if(me==0){
      fclose(fp);
    }
    cpu3 = clock();    // assign initial CPU time (IN CPU CLOCKS)
    gettimeofday(&time3, NULL); // returns structure with time in s and us (microseconds)
    dtime03 = ((time3.tv_sec  - time0.tv_sec)+(time3.tv_usec - time0.tv_usec)/1e6);
    printf("Elapsed wall time complete         CPU time\n");
    printf("Duration 03 %f %f\n", dtime03,(float) (cpu3-cpu0)/CLOCKS_PER_SEC);
  }/* End of Parallel Region */
}

