CC=gcc
CFLAGS=-Wall -std=c99
DEPS = value.h scanner.h node.h queue.h stack.h bst.h
OBJ = value.o scanner.o node.o queue.o stack.o bst.o calculon.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -g $<

calculon: $(OBJ)
	gcc $(CFLAGS) $(OBJ) -lm -o calculon

clean: 
	$(RM) count *.o *~
