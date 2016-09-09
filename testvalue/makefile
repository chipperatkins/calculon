OBJS = scanner.o value.o testvalue.o
OPTS = -Wall -g -std=c99

testvalue : $(OBJS)
		gcc $(OPTS) $(OBJS) -o testvalue -lm

testvalue.o : testvalue.c scanner.h value.h
		gcc $(OPTS) -c testvalue.c

scanner.o : scanner.c scanner.h
		gcc $(OPTS) -c scanner.c

value.o : value.c value.h
		gcc $(OPTS) -c value.c

clean :
		rm -f $(OBJS) testvalue
