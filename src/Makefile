CC = gcc
AS = nasm
LD = ld
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -nostdlib
LDFLAGS = -T kernel/linker.ld

KERNEL_SRC = kernel/kmain.c kernel/kernel.c
DRIVERS_SRC = drivers/fb.c drivers/cursor.c drivers/mouse.c drivers/time.c
OBJ = $(KERNEL_SRC:.c=.o) $(DRIVERS_SRC:.c=.o) boot/boot.o

all: os.iso

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

boot/boot.o: boot/boot.asm
    $(AS) -f elf32 $< -o $@

kernel.bin: $(OBJ)
    $(LD) $(LDFLAGS) -o $@ $^

os.iso: kernel.bin
    mkdir -p iso/boot/grub
    cp kernel.bin iso/boot/kernel.bin
    echo 'set timeout=0' > iso/boot/grub/grub.cfg
    echo 'set default=0' >> iso/boot/grub/grub.cfg
    echo 'menuentry "MiOS" { multiboot /boot/kernel.bin }' >> iso/boot/grub/grub.cfg
    grub-mkrescue -o os.iso iso

clean:
    rm -rf kernel/*.o drivers/*.o boot/*.o kernel.bin os.iso iso/
