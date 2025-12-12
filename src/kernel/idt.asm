global idt_load
section .text
idt_load:
    lidt [rdi]   ; carga la IDT desde el puntero pasado
    ret
