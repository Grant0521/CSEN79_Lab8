all: deque_test

deque_test: deque_test.o
	g++ -o deque_test deque_test.o

deque_test.o: deque_test.cpp deque.h deque_iterator.h
	g++ -c deque_test.cpp

clean:
	rm -f *.o deque_test