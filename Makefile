CC=g++
CFLAGS=-O2 -g0 -std=gnu++11 -pedantic -Wall -Wextra
LDFLAGS=-L/usr/lib64 -lX11
INCLUDES=-I/usr/local/include -I.

BinaryName=dungeon.app


OBJS=dungeon.o
all: $(OBJS) $(BinaryName)


%.o:        %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

$(BinaryName):
	$(CC) -o $(BinaryName) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(OBJS)

clean:
	rm -rf *.o $(BinaryName)