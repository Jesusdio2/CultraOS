# src/Makefile

CC = gcc
AS = nasm
LD = ld
CFLAGS = -m64 -ffreestanding -O2 -Wall -Wextra -nostdlib
LDFLAGS = -T linker.ld

SRC = kernel.c io.c fb.c cursor.c mouse.c time.c
OBJ = $(SRC:.c=.o)

all: kernel.bin

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

boot.o: boot.asm
    $(AS) -f elf64 $< -o $@

kernel.bin: boot.o $(OBJ)
    $(LD) $(LDFLAGS) -o $@ $^

clean:
    rm -f *.o kernel.bin
