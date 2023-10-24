
CC = gcc
CFLAGS = -Wall -Wextra -g -O3 -ljansson
LFLAGS = -ljansson
SRCS = $(wildcard *.c)

world-gen : $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $@ $(LFLAGS)

profile : $(SRCS)
	$(CC) $(CFLAGS) -pg $(SRCS) -o $@ $(LFLAGS)
