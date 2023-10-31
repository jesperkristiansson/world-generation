
CC = gcc
CFLAGS = -Wall -Wextra -g -O0 -ljansson
LFLAGS = -O0 -ljansson
SRC_DIR = ./src
SRCS = $(wildcard $(SRC_DIR)/*.c)

world-gen : $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $@ $(LFLAGS)

profile : $(SRCS)
	$(CC) $(CFLAGS) -pg $(SRCS) -o $@ $(LFLAGS)
