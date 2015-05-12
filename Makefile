export PATH := /home/member/ripp_/xscreensaver/hacks/:.:$(PATH)

S=/home/member/ripp_/xscreensaver/
H=$(S)hacks/
U=$(S)utils/

CC=gcc
CFLAGS=-O2 -g0 -pedantic -Wall -Wextra
LDFLAGS=-L/usr/lib64 -lX11
INCLUDES=-I/usr/local/include -I. -I/home/member/ripp_/xscreensaver/utils -I/home/member/ripp_/xscreensaver/hacks -I/home/member/ripp_/xscreensaver/hacks/glx

BinaryName=dungeon

OBJS=$(H)screenhack.o $(U)resources.o $(U)yarandom.o $(H)screenhack.c dungeon.o 
all: $(OBJS) $(BinaryName)


dungeon.o:        dungeon.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

$(BinaryName):
	$(CC) -o $(BinaryName) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(OBJS)

clean:
	rm -rf *.o $(BinaryName)