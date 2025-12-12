// src/kernel/multiboot.h
#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#define MULTIBOOT_HEADER_MAGIC 0x1BADB002
#define MULTIBOOT_HEADER_FLAGS 0x0
#define MULTIBOOT_HEADER_CHECKSUM -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

struct multiboot_header {
    unsigned int magic;
    unsigned int flags;
    unsigned int checksum;
} __attribute__((section(".multiboot")));

#endif
