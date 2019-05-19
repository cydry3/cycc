cycc: cycc.c

test: cycc
	./test.sh

clean:
	rm -f cycc *.o *~ tmp*


