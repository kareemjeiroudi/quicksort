#include <stdio.h>
#define max_len 100000
#define LENGTH 40

main(){
int i=1,len;
double bmax=-1e6,b,c,cold;
char name[LENGTH]="sorted.txt",line[LENGTH];
FILE *fp;

//printf("Input filename\n"); scanf("%s",name);
fp=fopen(name,"r");
while(1){ //1 serves as true, i.e. condition which is always true
//  printf("line %i\n",i);
  if(fgets(line, LENGTH,fp)==NULL)break; // finish reading when empty line is read
  if(sscanf(line, "%lf %lf",&b,&c)==-1) break; // finish reading after error
  if(b<bmax){printf("Number %lf in line %i is not in order\n",b,i); return 0;}
  if((b==bmax)&(cold>c)){printf("Second number %lf %lf in line %i is not in order\n",b,c,i); return 0;}
  i++;bmax=b;cold=c;
}
len=i-1;fclose(fp);
printf("Number of items sorted: %i\n",len);
}

