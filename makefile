CC=clang

CCFLAGS=-std=c11 -g -Wall
LDFLAGS=
IFLAGS=-Isrc -Isrc/configuration -Isrc/crypto
DEFINES=

CCFLAGS+=$(DEFINES)

ifdef ANQ_LOCAL_LIBS
LDFLAGS+=-L./usr/local/include/ -L./usr/local/lib/
endif

BIN_DIR=bin
SRC_DIR=src

OBJS=src/main.o \
     src/err_codes.o

include src/crypto/make.config
include src/configuration/make.config

.PHONY: prepare anq clean
.SUFFIXES: .c .o

all: prepare anq

prepare:
	if [ ! -z ${ANQ_LOCAL_LIBS} ]; then            \
		echo "./scripts/build-local-libs.sh";\
		./scripts/build-local-libs.sh;       \
	fi
	mkdir -p bin

.c.o:
	$(CC) -c $< -o $@ $(CCFLAGS) $(IFLAGS)

anq: $(OBJS)
	$(CC) $(OBJS) -o $(BIN_DIR)/$@ $(LDFLAGS)

clean:
	rm -f *.o */*.o */*/*.o */*/*/*.o
