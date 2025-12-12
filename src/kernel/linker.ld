ENTRY(kmain)

SECTIONS {
  . = 0x100000;

  /* Encabezado Multiboot2 al inicio */
  .multiboot ALIGN(8) : {
    KEEP(*(.multiboot))
  }

  /* Código */
  .text ALIGN(4K) : {
    *(.text*)
  }

  /* Datos de solo lectura */
  .rodata ALIGN(4K) : { *(.rodata*) }

  /* Datos inicializados */
  .data ALIGN(4K) : { *(.data*) }

  /* Datos no inicializados */
  .bss ALIGN(4K) : { *(COMMON) *(.bss*) }
}
