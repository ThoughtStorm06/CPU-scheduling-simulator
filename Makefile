CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Iinclude
SRCS = $(wildcard src/*.c)

ifeq ($(OS),Windows_NT)
	BIN = bin/scheduler.exe
	MKDIR = if not exist bin mkdir bin
	RM = del /Q
else
	BIN = bin/scheduler
	MKDIR = mkdir -p bin
	RM = rm -f
endif

all: $(BIN)

$(BIN): $(SRCS)
	$(MKDIR)
	$(CC) $(CFLAGS) -o $(BIN) $(SRCS)

clean:
	$(RM) $(BIN)

.PHONY: all clean