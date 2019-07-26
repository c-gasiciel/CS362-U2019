CFLAGS= -Wall -ftest-coverage -fprofile-arcs -lm -std=c99

default: runtests

testme: testme.c
	gcc -o testme -g testme.c $(CFLAGS)

runtests: testme
	./testme
