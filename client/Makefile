CC = gcc
CFLAGS = -Wall -g

all: client

client: main.o socket_client.o
    $(CC) $(CFLAGS) -o client main.o socket_client.o

main.o: main.c
    $(CC) $(CFLAGS) -c main.c

socket_client.o: socket_client.c
    $(CC) $(CFLAGS) -c socket_client.c

clean:
    rm -f *.o client
