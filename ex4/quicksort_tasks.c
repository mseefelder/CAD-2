#include "quicksort.h"
#include "quicksort_common.c"

void do_quicksort(int* values, size_t start, size_t end){
  size_t pivot;
  pivot = ip_partition(values, start, end);

  #pragma omp task
  if(pivot > start + 1){
    do_quicksort(values, start, pivot-1);
  }

  if(pivot < end-1){
    do_quicksort(values, pivot+1, end);
  }
}

void ip_quicksort(int* values, size_t start, size_t end) {
  #pragma omp parallel
  {
    #pragma omp single nowait
    do_quicksort(values, start, end);
    #pragma omp taskwait
  }
}
