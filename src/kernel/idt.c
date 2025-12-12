#include "idt.h"

// Descriptor de una entrada de la IDT (64 bits)
struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  ist;
    uint8_t  type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed));

// Puntero a la IDT
struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

#define IDT_ENTRIES 256

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idtp;

// Función externa en ASM para cargar la IDT
extern void idt_load(struct idt_ptr*);

// Configura una entrada de la IDT
void idt_set_isr(int vec, void (*handler)(void)) {
    uintptr_t addr = (uintptr_t)handler;
    idt[vec].offset_low  = addr & 0xFFFF;
    idt[vec].selector    = 0x08;     // selector de código kernel (GDT)
    idt[vec].ist         = 0;
    idt[vec].type_attr   = 0x8E;     // puerta de interrupción presente
    idt[vec].offset_mid  = (addr >> 16) & 0xFFFF;
    idt[vec].offset_high = (addr >> 32) & 0xFFFFFFFF;
    idt[vec].zero        = 0;
}

// Inicializa la IDT y la carga con lidt
void idt_init(void) {
    idtp.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
    idtp.base  = (uint64_t)&idt;
    idt_load(&idtp);
}
