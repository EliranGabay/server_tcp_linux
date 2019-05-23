CC=gcc
#CFLAGS=-O3

TARGETS=dict filestorage dirstorage

all: server

server: server.o dict.o filestorage.o dirstorage.o
	$(CC) -o server $^ -lpthread

%.o: %.c
	$(CC) -c $<

clean:
	rm -f $(TARGETS) *.o server

uninstall:
	sudo rm -f usr/local/bin/server

install:
	sudo cp server /usr/local/bin/	



