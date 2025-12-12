/* src/kernel/linker.ld */
ENTRY(kmain)

SECTIONS
{
  /* Dirección de carga del kernel */
  . = 0x100000;

  /* Código */
  .text ALIGN(4K) : {
    *(.multiboot)   /* cabecera multiboot */
    *(.text*)
  }

  /* Datos de solo lectura */
  .rodata ALIGN(4K) : {
    *(.rodata*)
  }

  /* Datos inicializados */
  .data ALIGN(4K) : {
    *(.data*)
  }

  /* Datos no inicializados (BSS) */
  .bss ALIGN(4K) : {
    *(COMMON)
    *(.bss*)
  }
}
