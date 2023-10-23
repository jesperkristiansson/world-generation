
CC = gcc
CFLAGS = -Wall -Wextra -g -O0
LFLAGS = 
SRCS = $(wildcard *.c)

main : $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $@ $(LFLAGS)

profile : $(SRCS)
	$(CC) $(CFLAGS) -pg $(SRCS) -o $@ $(LFLAGS)
