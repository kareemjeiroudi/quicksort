//sort.c
#include <time.h> // for CPU time
#include <sys/time.h> //for gettimeofday
#include <stdio.h>
#define max_len 400000
#define LENGTH 40

double find_minimum(double array[]) {
  double min=0;
  int length = (int) (sizeof(array)/sizeof(array[0]));
  for(int i=0;i<length;i++) {
    if(array[i] < min) {
      min = array[i];
    }
  }
  return min;
}

double find_maximum(double array[]) {
  double max=0;
  int length = (int) (sizeof(array)/sizeof(array[0]));
  for(int i=0;i<length;i++) {
    if(array[i] < max) {
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

int start, Nme, g, k, num_threads, range;
#pragma omp parallel shared(min, max, mid, qua_1st, qua_3rd, range) private(start, Nme, g, k)
{
  me = omp_get_thread_num();
  if (me==0) {
    // following values are to set the ranges of each thread according to the values
    double min = find_minimum(b);
    double max = find_maximum(b);
    double mid = (min+max)/2;
    double qua_1st = (min+mid)/2;
    double qua_3rd = (mid+max)/2;
    start = min;
    Nme = qua_1st;
  }
  else if(me==1) {
    start = qua_1st;
    Nme = mid;
  } 
  else if(me==2) {
    start = mid;
    Nme = qua_3rd;
  }
  else if(me==3) {
    start = qua_3rd;
    Nme = max;
  }
  int range = (int) (sizeof(b)/sizeof(b[0]))
  double b_me[range];
  for(g=)
}
len=i-1;fclose(fp);
printf("Number of items to sort: %i\n",len);
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
}

