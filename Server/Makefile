CFLAGS=-Wall -Wextra

all: command main


controller.o: controller.c
	gcc ${CFLAGS} -o controller.o -c controller.c -lpthread

%.o: %.c
	gcc ${CFLAGS} -o $@ -c $<

command : controller.o command.o parser.o liste.o
	gcc ${CFLAGS} controller.o command.o parser.o liste.o -o server -lpthread

main : main.o command.o parser.o liste.o
	gcc ${CFLAGS} main.o command.o parser.o liste.o -o main -lpthread

clean:
	rm -rf *.o server main
