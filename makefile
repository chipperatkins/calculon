CC=gcc
CFLAGS=-Wall -std=c99
DEPS = value.h scanner.h node.h queue.h stack.h 
OBJ = value.o scanner.o node.o queue.o stack.o calculon.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -g $<

calculon: $(OBJ)
	gcc $(CFLAGS) $(OBJ) -o calculon

clean: 
	$(RM) count *.o *~
