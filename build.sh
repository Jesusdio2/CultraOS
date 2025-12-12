#!/bin/bash
set -e

CC="gcc"
LD="ld"
CFLAGS="-m32 -ffreestanding -O2 -Wall -Wextra -nostdlib"
LDFLAGS="-T kernel/linker.ld"

$CC $CFLAGS -c kernel/kmain.c -o kernel/kmain.o
$CC $CFLAGS -c kernel/kernel.c -o kernel/kernel.o
$CC $CFLAGS -c drivers/fb.c -o drivers/fb.o
$CC $CFLAGS -c drivers/cursor.c -o drivers/cursor.o
$CC $CFLAGS -c drivers/mouse.c -o drivers/mouse.o
$CC $CFLAGS -c drivers/time.c -o drivers/time.o

$LD $LDFLAGS -o kernel.bin kernel/*.o drivers/*.o

mkdir -p iso/boot/grub
cp kernel.bin iso/boot/kernel.bin
echo 'set timeout=0' > iso/boot/grub/grub.cfg
echo 'set default=0' >> iso/boot/grub/grub.cfg
echo 'menuentry "MiOS" { multiboot /boot/kernel.bin }' >> iso/boot/grub/grub.cfg
grub-mkrescue -o os.iso iso
