CC=gcc
CFLAGS=-O2 -g0 -std=gnu++11 -pedantic -Wall -Wextra
LDFLAGS=-L/usr/lib64 -lX11
INCLUDES=-I/usr/local/include -I. -I./xscreensaver/utils -I./xscreensaver/hacks -I./xscreensaver/hacks/glx

BinaryName=dungeon

OBJS=dungeon.o
all: $(OBJS) $(BinaryName)


%.o:        %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

$(BinaryName):
	$(CC) -o $(BinaryName) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(OBJS)

clean:
	rm -rf *.o $(BinaryName)