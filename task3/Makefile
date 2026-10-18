CC = gcc
CFLAGS = -O2 -Wall -Wextra -std=gnu11 -pthread
LDFLAGS = -pthread

all: ll_my ll_lib

ll_my: pth_ll_rwl.c my_rwlock.c my_rand.c my_rwlock.h my_rand.h timer.h
	$(CC) $(CFLAGS) -DUSE_MY_RWLOCK -o $@ pth_ll_rwl.c my_rwlock.c my_rand.c $(LDFLAGS)

ll_lib: pth_ll_rwl.c my_rand.c my_rand.h timer.h
	$(CC) $(CFLAGS) -o $@ pth_ll_rwl.c my_rand.c $(LDFLAGS)

clean:
	rm -f ll_my ll_lib

.PHONY: all clean
