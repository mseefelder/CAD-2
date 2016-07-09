#include<stdio.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include<omp.h>
#include "quicksort.h"

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

float bench(size_t size){
  int* numbers = generate_numbers(size);

  clock_t start = clock();
  ip_quicksort(numbers,0,size-1);
  clock_t end = clock();
  float seconds = (float)(end - start) / CLOCKS_PER_SEC;

  size_t i;
  printf("Checking...\n");

  for(i = 1; i < size; i++){
    if(numbers[i] < numbers[i-1]){
      printf("Invalid ordering\n");
      break;
    }
    if(i == size-1){
      printf("Valid ordering\n");
    }
  }

  free(numbers);
  return seconds;
}

int main(int argc, char ** argv) {
  if(argc < 3){
    printf("Usage: ./quicksort k times\n");
    exit(1);
  }
  int k = atoi(argv[1]);
  int times = atoi(argv[2]);
  size_t size = (size_t) pow(2, (double) k);
  float seconds = 0;
  int i = 0;
  for(i = 0; i < times; i++){
    seconds += bench(size);
  }

  printf("Sorted %d times in %f seconds.\nMean: %f", times, seconds, seconds/times);
  return 0;
}
