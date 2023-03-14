CC = gcc
CFLAGS = -Wall -fsanitize=address
SOURCE = main.c
LIBS = -lm -lpthread
PROFILER = -pg

test:
	$(CC) $(CFLAGS) test.c -o test $(LIBS) $(PROFILER)
	./test