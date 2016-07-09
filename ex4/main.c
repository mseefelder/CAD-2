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

int main(int argc, char ** argv) {
  if(argc != 2){
    printf("Usage: ./quicksort k");
    exit(1);
  }
  int k = atoi(argv[1]);
  size_t size = (size_t) pow(2, (double) k);
  int* numbers = generate_numbers(size);
  size_t i;

  clock_t start = clock();
  ip_quicksort(numbers,0,size-1);
  clock_t end = clock();
  float seconds = (float)(end - start) / CLOCKS_PER_SEC;

  printf("Sorted in %f seconds.\n", seconds);
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
  return 0;
}
