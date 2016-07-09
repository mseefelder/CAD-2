#include "quicksort.h"
#include "quicksort_common.c"

void serial_quicksort(int* values, size_t start, size_t end){
  size_t pivot = ip_partition(values, start, end);

  if(pivot > start + 1){
    serial_quicksort(values, start, pivot-1);
  }

  if(pivot < end-1){
    serial_quicksort(values, pivot+1, end);
  }
}

void do_quicksort(int* values, size_t start, size_t end, int deep) {
  size_t pivot = ip_partition(values, start, end);
  if(deep <= 0){
    return serial_quicksort(values, start, end);
  }

  #pragma omp parallel
  {
    #pragma omp sections
    {
      #pragma omp section
      if(pivot > start + 1){
        do_quicksort(values, start, pivot-1, deep-1);
      }

      #pragma omp section
      if(pivot < end-1){
        do_quicksort(values, pivot+1, end, deep-1);
      }
    }
  }
}

void ip_quicksort(int* values, size_t start, size_t end){
  do_quicksort(values, start, end, 10);
}
