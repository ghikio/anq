CC=clang

CCFLAGS=-std=c11 -g -Wall -DDEBUG
LDFLAGS=
IFLAGS=-Isrc -Isrc/args

BIN_DIR=bin
SRC_DIR=src

OBJS=src/main.o

include src/args/make.config

.PHONY: prepare anq clean
.SUFFIXES: .c .o

all: prepare anq

prepare:
	mkdir -p bin

.c.o:
	$(CC) -c $< -o $@ $(CCFLAGS) $(IFLAGS)

anq: $(OBJS)
	$(CC) $(OBJS) -o $(BIN_DIR)/$@ $(LDFLAGS)

clean:
	rm -f *.o */*.o */*/*.o
