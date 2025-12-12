CC = gcc
LD = ld
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -nostdlib
LDFLAGS = -T kernel/linker.ld

KERNEL_SRC = kernel/kmain.c kernel/multiboot.c
OBJ = $(KERNEL_SRC:.c=.o)

all: os.iso

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

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
    rm -rf kernel/*.o kernel.bin os.iso iso/
