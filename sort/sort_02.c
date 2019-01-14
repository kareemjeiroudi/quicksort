//sort.c
#include <time.h> // for CPU time
#include <sys/time.h> //for gettimeofday
#include <stdio.h>
#include <omp.h>

#define max_len 400000
#define LENGTH 40

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

int range, num_threads, me;
#pragma omp parallel shared(range, num_threads, len, b, c) private(me, i, j, new, cnew, cur, fp)
{
  printf("HI\n");
  num_threads = omp_get_num_threads();
  me = omp_get_thread_num();
  range = len/num_threads;
  int start = me * range;
  int end = (me+1) * range;
  double b_me[range];
  double c_me[range];
  double ind_me[range];
  for(i=start;i<end;i++) {
    b_me[i] = b[i];
    c_me[i] = c[i];
  }

  ind[0]=1;
  for(j=2;j<=range;j++){ // start sorting with the second item
    new=b_me[j];cnew=c_me[j];cur=0;
    for(i=1;i<j;i++){
      prev=cur;cur=ind[cur];
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
  cur=0;
  if(me==0){fp=fopen("sorted_0.txt","w");}
  else if(me==1){fp=fopen("sorted_1.txt","w");}
  else if(me==2){fp=fopen("sorted_2.txt","w");}
  else if(me==3){fp=fopen("sorted_3.txt","w");}
  for(i=1;i<=range;i++){cur=ind_me[cur];fprintf(fp,"%lf %lf\n",b_me[cur],c_me[cur]);}
  fclose(fp);
  cpu3 = clock();    // assign initial CPU time (IN CPU CLOCKS)
  gettimeofday(&time3, NULL); // returns structure with time in s and us (microseconds)
  dtime03 = ((time3.tv_sec  - time0.tv_sec)+(time3.tv_usec - time0.tv_usec)/1e6);
  printf("Elapsed wall time complete         CPU time\n");
  printf("Duration 03 %f %f\n", dtime03,(float) (cpu3-cpu0)/CLOCKS_PER_SEC);
  }
}

