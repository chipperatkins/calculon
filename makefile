OBJS = scanner.o value.o node.o bst.o stack.o queue.o calculon.o
OPTS = -Wall -g -std=c99

calculon : $(OBJS)
	gcc $(OPTS) $(OBJS) -o calculon -lm

calculon.o : calculon.c stack.h queue.h bst.h scanner.h node.h value.h
	gcc $(OPTS) -c calculon.c

scanner.o : scanner.c scanner.h
	gcc $(OPTS) -c scanner.c

value.o : value.c value.h
	gcc $(OPTS) -c value.c

node.o : node.c node.h value.h
	gcc $(OPTS) -c node.c

bst.o : bst.c bst.h node.h value.h
	gcc $(OPTS) -c bst.c

stack.o : stack.c stack.h node.h value.h
	gcc $(OPTS) -c stack.c

queue.o : queue.c queue.h node.h value.h
	gcc $(OPTS) -c queue.c

clean :
	rm -f $(OBJS) calculon

