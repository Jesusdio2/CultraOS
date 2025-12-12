#ifndef MULTIBOOT2_H
#define MULTIBOOT2_H

#include <stdint.h>

// --- Header que GRUB busca en el binario ---
#define MULTIBOOT2_HEADER_MAGIC     0xE85250D6
#define MULTIBOOT2_ARCHITECTURE     0   // 0 = i386, válido para x86_64
#define MULTIBOOT2_HEADER_LENGTH    16
#define MULTIBOOT2_CHECKSUM -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT2_ARCHITECTURE + MULTIBOOT2_HEADER_LENGTH)

struct multiboot2_header {
    uint32_t magic;
    uint32_t architecture;
    uint32_t header_length;
    uint32_t checksum;
};

// --- Valores que GRUB pasa al kernel ---
#define MULTIBOOT2_BOOTLOADER_MAGIC 0x36d76289

// Tipos de tag
#define MULTIBOOT_TAG_TYPE_END         0
#define MULTIBOOT_TAG_TYPE_FRAMEBUFFER 8

// Tag genérico
struct multiboot_tag {
    uint32_t type;
    uint32_t size;
};

// Tag específico para framebuffer
struct multiboot_tag_framebuffer {
    uint32_t type;
    uint32_t size;
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t  framebuffer_bpp;
    uint8_t  framebuffer_type;
    uint16_t reserved;
};

#endif
