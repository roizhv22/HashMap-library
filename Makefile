.PHONY : all clean

CCFLAGS = -Wall -Wextra -Wvla -Werror -g -lm -std=c99
CC = gcc
LIB_STANDARD_OBJECTS = vector.o hashmap.o pair.o
LIB_TESTS_OBJECTS = vector.o hashmap.o pair.o test_suite.o test_pairs.h hash_funcs.h

all: $(LIB_TESTS_OBJECTS)
	ar rcs libhashmap.a $(LIB_STANDARD_OBJECTS)
	ar rcs libhashmap_tests.a $(LIB_TESTS_OBJECTS)


libhashmap.a: $(LIB_STANDARD_OBJECTS)
	ar rcs libhashmap.a $^

libhashmap_tests.a: $(LIB_TESTS_OBJECTS)
	ar rcs libhashmap_tests.a $^

vector.o: vector.c vector.h
	$(CC) $(CCFLAGS) -c $<

pair.o: pair.c pair.h
	$(CC) $(CCFLAGS) -c $<

hashmap.o: hashmap.c hashmap.h
	$(CC) $(CCFLAGS) -c $<

test_suite.o: test_suite.c test_suite.h
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f *.o *.a

