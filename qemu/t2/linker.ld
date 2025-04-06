OUTPUT_ARCH("riscv")
ENTRY(_start)

MEMORY
{
    /* QEMU virt machine memory map */
    RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 128M
    /* The (rwx) here defines default permissions if not overridden by PHDRS,
       but also defines the maximum allowed permissions for sections placed here. */
}

SECTIONS
{
  . = 0x80000000;
  .text : {
      *(.text.*)
      *(.text)
  }
  .rodata : { *(.rodata) }
  .data : { *(.data) }
  .bss  : { *(.bss) }
  /* . = ALIGN(8); */
  /* PROVIDE(_stack_top = .); */
}

