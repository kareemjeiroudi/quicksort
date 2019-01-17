//sort.c
#include <time.h> // for CPU time
#include <sys/time.h> //for gettimeofday
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define max_len 400000
#define LENGTH 40

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

int main(int argc, char **argv){
int i=1,len,ind[max_len+1],j,cur,prev;
double b[max_len+1],c[max_len+1],new,cnew,time;
char name[LENGTH]="sort.txt",line[LENGTH];
FILE *fp;
clock_t cpu0,cpu1,cpu2,cpu3; // clock_t defined in <time.h> and <sys/types.h> as int
struct timeval time0, time1,time2,time3; // for wall clock in s and us
double  dtime12,dtime03; // for wall clock in s (real number)

int me, nproc, ierr;
MPI_Init(&ierr, &argv); // initialise MPI
MPI_Comm_size(MPI_COMM_WORLD, &nproc); // return total number of processors
MPI_Comm_rank(MPI_COMM_WORLD, &me); // return number of this processor, rank=0..nproc-1

if (me ==0) {
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
  for(i=0; i<= len; i++) {
    printf("b[%d] = %lf\n", i, b[i]);
  }
}

// Communication and global calculation
MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);
MPI_Bcast(&b[0], len+1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
MPI_Bcast(&c[0], len+1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
double max = find_maximum(b, len+1);
double min = find_minimum(b, len+1);

// just to assure correct communication among the threads
// if(me==3) {
//   printf("I'm thread %d, I didn't read the file!\nAnd I calculated the following max = %lf and min = %lf", me, max, min);
//   for (i=0; i< len+1; i++) {
//     printf("%lf\n", b[i]);
//   }
//   printf("\n");
// }
/* Initiate Parallelization */
double *b_me_neg = malloc(sizeof(double)*len);
double *c_me_neg = malloc(sizeof(double)*len);
int *ind_me_neg = malloc(sizeof(int)*len);
double *b_me_pos = malloc(sizeof(double)*len);
double *c_me_pos = malloc(sizeof(double)*len);
int *ind_me_pos = malloc(sizeof(int)*len);
// start and end of negative numbers
double start_neg = me * (min/nproc +1) - me;
double end_neg = (me+1) * (min/nproc +1) - (me+1);
// start and end of positive numbers
double start_pos = me * (max/nproc +1) - me;
double end_pos = (me+1) * (max/nproc +1) - (me+1);
// counts of each size of number to each thread
int pos=0, neg=0, count=0;

b_me_neg[0] = 0.0;
b_me_pos[0] = 0.0;
// take only negative numbers
for(i=1;i<=len;i++) {
  if((b[i] < 0.0 && b[i] <= start_neg && b[i] > end_neg) || (b[i] == end_neg && end_neg == min)) {
    b_me_neg[neg] = b[i];
    c_me_neg[neg] = c[i];
    neg++;
  }
  else if((b[i] >= 0.0 && b[i] >= start_pos && b[i] < end_pos) || (b[i] == end_pos &&  end_pos == max)) {
    b_me_pos[pos] = b[i];
    c_me_pos[pos] = c[i];
    pos++;
  }
}
count = pos + neg;
printf("Thread %d negatively start_neg = %lf end_neg = %lf\n\
  and positively start_pos = %lf end_pos = %lf\n\
  pos = %d , neg = %d , count =%d\n", me, start_neg, end_neg, start_pos, end_pos, pos, neg, count);
MPI_Barrier(MPI_COMM_WORLD);
if(me==0)printf("\n"); // line Break for nice output


for(i=1; i<=neg; i++) {
  printf("Thread %d b_me_neg[%d] = %lf\n", me, i, b_me_neg[i]);
}
MPI_Barrier(MPI_COMM_WORLD);
for(i=1; i<=pos; i++) {
  printf("Thread %d b_me_pos[%d] = %lf\n", me, i, b_me_pos[i]);
}
MPI_Barrier(MPI_COMM_WORLD);
if(me==0) printf("\n\n\n");
MPI_Finalize(); // finalize MPI peacefully (the system would kill the processes otherwise)

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
cur=0;
fp=fopen("sorted.txt","w");
for(i=1;i<=len;i++){cur=ind[cur];fprintf(fp,"%lf %lf\n",b[cur],c[cur]);}
fclose(fp);
cpu3 = clock();    // assign initial CPU time (IN CPU CLOCKS)
gettimeofday(&time3, NULL); // returns structure with time in s and us (microseconds)
dtime03 = ((time3.tv_sec  - time0.tv_sec)+(time3.tv_usec - time0.tv_usec)/1e6);
printf("Elapsed wall time complete         CPU time\n");
printf("Duration 03 %f %f\n", dtime03,(float) (cpu3-cpu0)/CLOCKS_PER_SEC);
return 0;
}

