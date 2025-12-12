#ifndef MULTIBOOT2_H
#define MULTIBOOT2_H

#define MULTIBOOT2_HEADER_MAGIC 0xE85250D6
#define MULTIBOOT2_ARCHITECTURE 0   // 0 = i386, válido para x86_64
#define MULTIBOOT2_HEADER_LENGTH 16
#define MULTIBOOT2_CHECKSUM -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT2_ARCHITECTURE + MULTIBOOT2_HEADER_LENGTH)

struct multiboot2_header {
    unsigned int magic;
    unsigned int architecture;
    unsigned int header_length;
    unsigned int checksum;
} __attribute__((section(".multiboot")));

#endif
