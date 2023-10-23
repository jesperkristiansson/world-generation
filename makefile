
CC = gcc
CFLAGS = -Wall -Wextra -g -O0 -ljansson
LFLAGS = -ljansson
SRCS = $(wildcard *.c)

main : $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $@ $(LFLAGS)

profile : $(SRCS)
	$(CC) $(CFLAGS) -pg $(SRCS) -o $@ $(LFLAGS)
