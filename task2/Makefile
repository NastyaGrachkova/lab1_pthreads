CC = gcc
CFLAGS = -O2 -Wall -Wextra -std=gnu11 -pthread
LDFLAGS = -pthread -lm

all: nbody gen_input

nbody: nbody.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

gen_input: gen_input.c my_rand.c my_rand.h
	$(CC) $(CFLAGS) -o $@ gen_input.c my_rand.c

clean:
	rm -f nbody gen_input

run: nbody gen_input
	mkdir -p inputs results
	test -f inputs/test_100.txt || ./gen_input 100 inputs/test_100.txt
	./nbody 10.0 inputs/test_100.txt 4 0.01 results/out.csv

.PHONY: all clean run
