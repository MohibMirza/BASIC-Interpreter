CC=g++

CFLAGS=-g -Wall -c -std=c++11

all: hw4

hw4: main.cpp interpreter.o arithmetic.o command.o booleanvar.o
	$(CC) main.cpp interpreter.o arithmetic.o command.o booleanvar.o -o hw4

interpreter.o: interpreter/interpreter.h interpreter/interpreter.cpp
	$(CC) $(CFLAGS) interpreter/interpreter.cpp -o interpreter.o

arithmetic.o: interpreter/arithmetic.h interpreter/arithmetic.cpp
	$(CC) $(CFLAGS) interpreter/arithmetic.cpp -o arithmetic.o

command.o: interpreter/command.h interpreter/command.cpp
	$(CC) $(CFLAGS) interpreter/command.cpp -o command.o

booleanvar.o: interpreter/booleanvar.h interpreter/booleanvar.cpp
	$(CC) $(CFLAGS) interpreter/booleanvar.cpp -o booleanvar.o