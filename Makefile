CC=gcc
CFLAGS=-g -Wall

default: ksrs

ksrs: ./src/*.c
	$(CC) $(CFLAGS) -o ./build/$@ $^

clean:
	rm -f ./build/ksrs
