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
double dtime12, dtime03; // for wall clock in s (real number)

cpu0 = clock();    // assign initial CPU time (IN CPU CLOCKS)
gettimeofday(&time0, NULL); // returns structure with time in s and us (microseconds)
//printf("Input filename\n"); scanf("%s",name);
fp=fopen(name,"r");
while(1){ //1 serves as true, i.e. condition which is always true
  if(fgets(line, LENGTH,fp)==NULL)break; // finish reading when empty line is read
  if(sscanf(line, "%lf %lf",&b[i],&c[i])==-1) break; // finish reading after error
  i++;
} /* while end */

len=i-1;fclose(fp);
printf("Number of items to sort: %i\n",len);
cpu1 = clock();    // assign initial CPU time (IN CPU CLOCKS)
gettimeofday(&time1, NULL); // returns structure with time in s and us (microseconds)
ind[0]=1;
for(j=2;j<=len;j++){ // start sorting with the second item
  new=b[j];cnew=c[j];cur=0;
  #pragma omp parallel for private(i, prev, cur) auto
  for(i=1;i<j;i++){
    prev=cur;cur=ind[cur];
    if(new==b[cur]){printf("Equal numbers %lf\n",new);}
    if((new<b[cur]) | ((new==b[cur])&(cnew<c[cur])))
    {
      ind[prev]=j;ind[j]=cur;goto loop;
    }
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

