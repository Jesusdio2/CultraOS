#!/bin/bash
set -e

CC="gcc"
LD="ld"
CFLAGS="-m32 -ffreestanding -O2 -Wall -Wextra -nostdlib -Isrc/drivers -Isrc/kernel"
LDFLAGS="-T src/kernel/linker.ld"

# Compilar fuentes del kernel
$CC $CFLAGS -c src/kernel/kmain.c -o src/kernel/kmain.o
$CC $CFLAGS -c src/kernel/kernel.c -o src/kernel/kernel.o
$CC $CFLAGS -c src/kernel/console.c -o src/kernel/console.o   # <-- agregado

# Compilar drivers
$CC $CFLAGS -c src/drivers/fb.c -o src/drivers/fb.o
$CC $CFLAGS -c src/drivers/cursor.c -o src/drivers/cursor.o
$CC $CFLAGS -c src/drivers/mouse.c -o src/drivers/mouse.o
$CC $CFLAGS -c src/drivers/time.c -o src/drivers/time.o

# Enlazar kernel
$LD $LDFLAGS -o src/kernel.bin src/kernel/*.o src/drivers/*.o

# Crear ISO con GRUB
mkdir -p src/iso/boot/grub
cp src/kernel.bin src/iso/boot/kernel.bin
echo 'set timeout=0' > src/iso/boot/grub/grub.cfg
echo 'set default=0' >> src/iso/boot/grub/grub.cfg
echo 'menuentry "CultraCore" { multiboot /boot/kernel.bin }' >> src/iso/boot/grub/grub.cfg
grub-mkrescue -o src/os.iso src/iso
