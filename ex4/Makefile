all: quicksort serial_quicksort

quicksort: quicksort.c
	gcc quicksort.c -fopenmp -lm -o quicksort

serial_quicksort: quicksort.c
	gcc quicksort.c -lm -o serial_quicksort
