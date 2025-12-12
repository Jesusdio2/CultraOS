#include "multiboot2.h"

struct multiboot2_header mb2_header __attribute__((section(".multiboot"))) = {
    MULTIBOOT2_HEADER_MAGIC,
    MULTIBOOT2_ARCHITECTURE,
    MULTIBOOT2_HEADER_LENGTH,
    MULTIBOOT2_CHECKSUM
};
