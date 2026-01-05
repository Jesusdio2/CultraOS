# src/Makefile
CC := gcc
CFLAGS := -O2 -Wall -Wextra
LDFLAGS := -lpng -lz

BIN := ui
SRC := main.c

all: $(BIN)

$(BIN): $(SRC)
    $(CC) $(CFLAGS) -o $(BIN) $(SRC) $(LDFLAGS)

clean:
    rm -f $(BIN) *.o
