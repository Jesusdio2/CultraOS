// src/kernel/multiboot.c
#include "multiboot.h"

struct multiboot_header mb_header __attribute__((section(".multiboot"))) = {
    MULTIBOOT_HEADER_MAGIC,
    MULTIBOOT_HEADER_FLAGS,
    MULTIBOOT_HEADER_CHECKSUM
};
