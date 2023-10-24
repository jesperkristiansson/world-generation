
CC = gcc
CFLAGS = -Wall -Wextra -g -O3 -ljansson
LFLAGS = -ljansson
SRC_DIR = ./src
SRCS = $(wildcard $(SRC_DIR)/*.c)

world-gen : $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $@ $(LFLAGS)

profile : $(SRCS)
	$(CC) $(CFLAGS) -pg $(SRCS) -o $@ $(LFLAGS)
