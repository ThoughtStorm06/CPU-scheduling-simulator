CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Iinclude
SRCS = $(wildcard src/*.c)
BIN = bin/scheduler

all: $(BIN)

$(BIN): $(SRCS)
	@mkdir -p bin || if not exist bin mkdir bin
	$(CC) $(CFLAGS) -o $(BIN) $(SRCS)

clean:
	rm -f $(BIN) build/*.o

.PHONY: all clean
