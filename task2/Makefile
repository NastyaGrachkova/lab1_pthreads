CC = gcc
CFLAGS = -O2 -Wall -Wextra -std=gnu11 -pthread
LDFLAGS = -pthread -lm

all: nbody

nbody: nbody.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f nbody

run: nbody
	./nbody 10.0 inputs/test_100.txt 4 0.01 results/out.csv

.PHONY: all clean run
