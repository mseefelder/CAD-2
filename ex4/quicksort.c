#include <stdio.h>
#include <math.h>
#include <sys/time.h>
//#include <time.h>
#include <stdlib.h>
#include <omp.h>

inline void swap(int* values, size_t i, size_t j){
  int temp = values[i];
  values[i] = values[j];
  values[j] = temp;
}

// Use the last element as the pivot always
size_t ip_partition(int* values, size_t start, size_t end) {
  size_t first_bigger = start;
  size_t i;

  for(i = start; i < end; i++) {
    if(values[i] < values[end]){
      swap(values, i, first_bigger);
      first_bigger++;
    }
  }
  swap(values, first_bigger, end);

  // Now the pivot is in the position of the first bigger
  return first_bigger;
}

void ip_quicksort(int* values, size_t start, size_t end) {
  size_t pivot;
  pivot = ip_partition(values, start, end);

  #pragma omp parallel
  {
    #pragma omp sections
    {
      #pragma omp section
      if(pivot > start + 1){
        ip_quicksort(values, start, pivot-1);
      }

      #pragma omp section
      if(pivot < end-1){
        ip_quicksort(values, pivot+1, end);
      }
    }
  }
}


int* generate_numbers(size_t size) {
  size_t i;
  time_t t;
  int* numbers = malloc(size * sizeof(int));

  srand((unsigned) time(&t));
  for(i=0; i < size; i++) {
    numbers[i] = rand();
  }

  return numbers;
}

int main(int argc, char ** argv) {
  if(argc != 2){
    printf("Usage: ./quicksort k");
    exit(1);
  }
  int k = atoi(argv[1]);
  size_t size = (size_t) pow(2, (double) k);
  int* numbers = generate_numbers(size);
  size_t i;

  struct timeval start, end;
  gettimeofday(&start, NULL);
  //clock_t start = clock();
  
  ip_quicksort(numbers,0,size-1);
  
  gettimeofday(&end, NULL);
  float seconds = ((end.tv_sec  - start.tv_sec) * 1000000u + 
         end.tv_usec - start.tv_usec) / 1.e6;
  //clock_t end = clock();
  //float seconds = (float)(end - start) / CLOCKS_PER_SEC;

  printf("Sorted in %f seconds.\n", seconds);
  free(numbers);
}
