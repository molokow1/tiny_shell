
CC=gcc 
CFLAGS= -c -Wall
PROG_NAME = tiny_shell
all: main 

main: main.o
	$(CC) main.o -o $(PROG_NAME)

main.o: main.c
	$(CC) $(CFLAGS) main.c


run: $(PROG_NAME)
	./$(PROG_NAME)

clean: 
	rm *.o $(PROG_NAME)

