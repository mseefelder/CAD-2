#ifndef MAX_DEEP
#define MAX_DEEP 15
#endif

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

void serial_quicksort(int* values, size_t start, size_t end){
  size_t pivot = ip_partition(values, start, end);

  if(pivot > start + 1){
    serial_quicksort(values, start, pivot-1);
  }

  if(pivot < end-1){
    serial_quicksort(values, pivot+1, end);
  }
}
