section .multiboot_header
align 8
multiboot2_header:
    dd 0xE85250D6              ; MULTIBOOT2_HEADER_MAGIC
    dd 0                      ; architecture (0 = i386)
    dd multiboot2_header_end - multiboot2_header ; header length
    dd -(0xE85250D6 + 0 + (multiboot2_header_end - multiboot2_header)) ; checksum

multiboot2_header_end:
