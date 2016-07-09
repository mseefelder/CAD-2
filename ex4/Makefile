CC = gcc
CFLAGS = -Wall -O2
LFLAGS = -lm
OMPFLAGS = -fopenmp

all: sections serial tasks

%.o: %.c quicksort_common.c
	$(CC) $(CFLAGS) $(OMPFLAGS) -c $< -o $@

sections: main.o quicksort_sections.o
	$(CC) $(CFLAGS) $(OMPFLAGS) $^ -o $@ $(LFLAGS)

tasks: main.o quicksort_tasks.o
	$(CC) $(CFLAGS) $(OMPFLAGS) $^ -o $@ $(LFLAGS)

serial: main.c quicksort_sections.c
	$(CC) -O2 $^ -o $@ $(LFLAGS)
