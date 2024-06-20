CC = gcc
CFLAGS = -Wall -g

all: server

server: main.o admin.o customer.o ipc.o
    $(CC) $(CFLAGS) -o server main.o admin.o customer.o ipc.o

main.o: main.c
    $(CC) $(CFLAGS) -c main.c

admin.o: admin.c
    $(CC) $(CFLAGS) -c admin.c

customer.o: customer.c
    $(CC) $(CFLAGS) -c customer.c

ipc.o: ipc.c
    $(CC) $(CFLAGS) -c ipc.c

clean:
    rm -f *.o server
