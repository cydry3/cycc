cycc: cycc.c

test: cycc
	./cycc -test
	./test.sh

clean:
	rm -f cycc *.o *~ tmp*


