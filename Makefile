systemc: main.o queue.o undo.o txt_utils.o
	gcc main.o queue.o undo.o txt_utils.o -o systemc

main.o: main.c
	gcc -c main.c

queue.o: queue.c queue.h
	gcc -c queue.c

undo.o: undo.c undo.h
	gcc -c undo.c

txt_utils.o: txt_utils.c txt_utils.h
	gcc -c txt_utils.c

clean:
	rm *.o systemc
