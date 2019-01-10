//sort.c
#include <time.h> // for CPU time
#include <sys/time.h> //for gettimeofday
#include <stdio.h>
#include <omp.h>

#define max_len 400000
#define LENGTH 40

int find_length(double array[]) {
  return 0;
}

double find_maximum(double array[], int n) {
  int maximum = 0, i=0;
  for(int i=0; i<n;i++) {
    if(array[i] > maximum) {
      maximum = array[i]
    }
  }
  return maximum;
}

double find_minimum(double array[], int n) {
  int minimum = 0, i=0;
  for(int i=0; i<=n;i++) {
    if(array[i] < minimum) {
      minimum = array[i]
    }
  }
  return minimum;
}


main() {
  int i=1,len,ind[max_len+1],j,cur,prev, num_threads;
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

  len=i-1; fclose(fp);
  printf("Number of items to sort: %i\n",len);
  /* Beginning of parallel region */
  num_threads = omp_get_num_threads();
  #pragma omp parallel private(i, me, Nme, g) shared(num_threads, range) ordered
  {
    me = omp_get_thread_num();
    int range = (len+1)/num_threads // range is number of elements to be sorted by each thread
    i = me * range, g = 0; // i is starting point where each thread should start storing elements from original array
    int Nme = (me+1) * range; // Nme is the ending element where each thread should end storing elements from original array
    int b_me[range], c_me[range]; // b_me c_me are equivelent to b[] and c[] but contain only those element that thread n is responisble for
    for(i; i<Nme; i++) {
      b_me[g] = b[i];
      c_me[g] = c[i];
      g++;
    }
    for(g=0;g<range; g++) {
      printf("%d\n", b_me[g]);
    }
    cpu1 = clock();    // assign initial CPU time (IN CPU CLOCKS)
    gettimeofday(&time1, NULL); // returns structure with time in s and us (microseconds)
    ind[0]=1;

    /* Begin of sorting (is parallelized) */ 
    #pragma omp parallel for private(i, me, prev, cur, new) shared(ind)
    for(j=2;j<=len;j++){ // start sorting with the second item
      new=b[j];cnew=c[j];cur=0;
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
    } /* end of sorting */

    cpu2 = clock();    // assign CPU time (IN CPU CLOCKS)
    gettimeofday(&time2, NULL);
    dtime12 = ((time2.tv_sec  - time1.tv_sec)+(time2.tv_usec - time1.tv_usec)/1e6);
    printf("Elapsed wall time sorting         CPU time\n");
    printf("Duration 12 %f %f\n", dtime12,(float) (cpu2-cpu1)/CLOCKS_PER_SEC);
    cur=0;

    /* Being of writing */
    if(me==0) fp=fopen("sorted.txt", "w");
    else fp=fopen("sorted.txt", "a");
    for(i=1;i<=Nme;i++){
      cur=ind[cur];fprintf(fp,"%lf %lf\n",b[cur],c[cur]);
    }
    fclose(fp); /*End of Writing */

    cpu3 = clock();    // assign initial CPU time (IN CPU CLOCKS)
    gettimeofday(&time3, NULL); // returns structure with time in s and us (microseconds)
    dtime03 = ((time3.tv_sec  - time0.tv_sec)+(time3.tv_usec - time0.tv_usec)/1e6);
    printf("Elapsed wall time complete         CPU time\n");
    printf("Duration 03 %f %f\n", dtime03,(float) (cpu3-cpu0)/CLOCKS_PER_SEC);
  
  } /* End of Parallel Region */
}

