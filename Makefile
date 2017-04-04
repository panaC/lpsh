
CC=gcc
CFLAGS=-Wall -Wextra -Werror
LDFLAGS=-g
EXEC=lpsh

all: mrproper lpsh

lpsh: fonc.o main.o
	$(CC) -o $(EXEC) fonc.o main.o $(LDFLAGS)

fonc.o: fonc.c
	$(CC) -o fonc.o -c fonc.c $(CFLAGS)

main.o: main.c fonc.h
	$(CC) -o main.o -c main.c $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
