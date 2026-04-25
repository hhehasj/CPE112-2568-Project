RUN: main.o queue.o undo.o
	gcc main.o queue.o undo.o -o RUN

main.o: main.c
	gcc -c main.c

queue.o: queue.c queue.h
	gcc -c queue.c

undo.o: undo.c undo.h
	gcc -c undo.c

clean:
	rm *.o output
