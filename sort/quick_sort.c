#include <stdio.h>

#define max_len 400000
#define LENGTH 80


void partitionArray(double *b, double *c, int beg, int end, int *pivotLoc);
void quickSort(double *b, double *c, int beg, int end);

int main(){
  int i=0,len;
  double b[max_len+1];
  // double *b = malloc(sizeof(double)*max_len+1),
  double c[max_len+1];
  char name[LENGTH]="sort.txt",line[LENGTH];
  FILE *fp;

  fp=fopen(name,"r");
  while(1){ //1 serves as true, i.e. condition which is always true
    if(fgets(line, LENGTH,fp)==NULL)break; // finish reading when empty line is read
    if(sscanf(line, "%lf %lf",&b[i],&c[i])==-1) break; // finish reading after error
    i++;
  }

  len=i;fclose(fp);
  printf("Number of items to sort: %i\n",len);
  

  quickSort(b, c, 0, len-1);  //beg = 0 start index of array end = 5 last index of array

  // writing sorted element of both arrays to sorted.txt
  fp = fopen("sorted.txt", "w");
  for(i = 0; i < len; i++){
    fprintf(fp, "%lf %lf\n", b[i], c[i]);
  }
  fclose(fp);
  
  return 0;
}//main() ends here

void quickSort(double *b, double *c, int beg, int end){
  int pivotLoc;
  if(beg < end){
    partitionArray(b, c, beg, end, &pivotLoc);  //this will find the pivot location and partition the array
    quickSort(b, c, beg, pivotLoc - 1);  //quick sort the left sub array
    quickSort(b, c, pivotLoc + 1, end);  //quick sort the right sub array
  }
}//quickSort() ends here

void partitionArray(double *b, double *c, int beg, int end, int *pivotLoc){
  int left = beg;    //initially left point to the first element of the array
  int right = end;  //initially right point to the last element of the array
  *pivotLoc = left;  //initially pivot point to first element of the array
  double tmp, tmp_c;  //used for swapping values

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
      tmp_c = c[right];
      b[right] = b[*pivotLoc];
      c[right] = c[*pivotLoc];
      b[*pivotLoc] = tmp;
      c[*pivotLoc] = tmp_c;
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
      tmp_c = c[left];
      b[left] = b[*pivotLoc];
      c[left] = c[*pivotLoc];
      b[*pivotLoc] = tmp;
      c[*pivotLoc] = tmp_c;
      *pivotLoc = left;  //pivot is now pointing to left
    }
  }
}//partitionArray() ends here