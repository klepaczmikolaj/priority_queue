CC = gcc

all: test simulation

simulation: priority_queue.o main.o
	$(CC) priority_queue.c main.c -o simulation

test: priority_queue.o test.o
	$(CC) priority_queue.c test.c -o test

clean: 
	-rm *.o test simulation