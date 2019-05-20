CFLAGS=-Wall -std=c11
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

cycc: $(OBJS)
	$(CC) -o cycc $(OBJS) $(LDFLAGS)

$(OBJS): cycc.h

test: cycc
	./cycc -test
	./test.sh

clean:
	rm -f cycc *.o *~ tmp*


