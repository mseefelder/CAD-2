#include "quicksort.h"
#include "quicksort_common.c"
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
