RUN: main.o queue.o
	gcc main.o queue.o -o RUN

main.o: main.c
	gcc -c main.c

queue.o: queue.c queue.h
	gcc -c queue.c

clean:
	rm *.o output